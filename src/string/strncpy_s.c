/*
 * Copyright (c) 2017 Juniper Networks.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "libc_private.h"

/* ISO/IEC 9899:2011 K.3.7.1.4 */
errno_t strncpy_s(char * __restrict s1, rsize_t s1max,
    const char * __restrict s2, rsize_t n)
{
	rsize_t lim;
	uintptr_t s1e, s2e;
	errno_t ret;
	int zero_fill;
	size_t s, t;

	ret = EINVAL;
	zero_fill = 0;
	if (s1 == NULL) {
		__throw_constraint_handler_s("strncpy_s : s1 is NULL", ret);
	} else if (s1max > RSIZE_MAX) {
		__throw_constraint_handler_s("strncpy_s : s1max > RSIZE_MAX",
		    ret);
	} else if (s1max == 0) {
		__throw_constraint_handler_s("strncpy_s : s1max == 0", ret);
	} else if (s2 == NULL) {
		zero_fill = 1;
		__throw_constraint_handler_s("strncpy_s : s2 is NULL", ret);
	} else {
		lim = s1max;
		/* assume s2 size will be ok */
		s = n;
		/*
		 * reset s2 size if n > s1max
		 *
		 * the '=' handles this corner case:
		 * n == s1max and s2[n-1] != 0
		 * then s1 will not be null terminated
		 */
		if (n >= s1max) {
			/*
			 * number of char's
			 * slmax -1 to account for the +1 added to result for
			 * null terminator.
			 */
			s = strnlen_s(s2, s1max - 1);
			/* +1 to include null terminator */
			s++;
		}
		if (s < s1max)
			lim = s;
		if (lim > 0) {
			s1e = (uintptr_t) s1 + lim;
			s2e = (uintptr_t) s2 + lim;
			if (((uintptr_t) s1 >= (uintptr_t) s2 &&
			     (uintptr_t) s1 < s2e) ||
			    ((uintptr_t) s2 >= (uintptr_t) s1 &&
			     (uintptr_t) s2 < s1e)) {
				/* do not zero fill on overlapping starts */
				if (s1 != s2)
					zero_fill = 1;
				__throw_constraint_handler_s(
				    "strncpy_s : memory overlaps", ret);
			} else {
				if (s == s1max && s2[s1max -1] != 0) {
					zero_fill = 1;
					/*
					 * Do not have a special throw string
					 * for n == s1max corner case
					 */
					__throw_constraint_handler_s(
					    "strncpy_s : s2 will be truncated",
					    ret);
				} else {
					/* strncpy does not null terminate. */
					for (t = 0; t < lim; t++) {
						if (s2[t] == 0)
							break;
						s1[t] = s2[t];
					}
					s1[t] = 0;
					ret = 0;
				}
			}
		} else {
			/* trivial, still need to null terminate */
			s1[0] = 0;
			ret = 0;
		}
	}
	if (ret && zero_fill)
		s1[0] = 0;
	return (ret);
}
