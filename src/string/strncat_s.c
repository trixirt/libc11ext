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

/* ISO/IEC 9899:2011 K.3.7.2.2 */
errno_t strncat_s(char * __restrict s1, rsize_t s1max,
    const char * __restrict s2, rsize_t n)
{
	rsize_t lim;
	uintptr_t s1e, s2e;
	errno_t ret;
	size_t m, o;
	int zero_fill;

	ret = EINVAL;
	zero_fill = 0;
	if (s1 == NULL) {
		__throw_constraint_handler_s("strncat_s : s1 is NULL", ret);
	} else if (s1max > RSIZE_MAX) {
		__throw_constraint_handler_s("strncat_s : s1max > RSIZE_MAX",
		    ret);
	} else if (s1max == 0) {
		__throw_constraint_handler_s("strncat_s : s1max == 0", ret);
	} else if (s2 == NULL) {
		zero_fill = 1;
		__throw_constraint_handler_s("strncat_s : s2 is NULL", ret);
	} else if (n > RSIZE_MAX) {
		zero_fill = 1;
		__throw_constraint_handler_s("strncat_s : n > RSIZE_MAX", ret);
	} else {
		m = s1max - strnlen_s(s1, s1max);
		if (m == 0) {
			zero_fill = 1;
			__throw_constraint_handler_s(
			    "strncat_s : s1 is full", ret);
		} else {
			o = strnlen_s(s2, n);
			if (o >= m) {
				zero_fill = 1;
				__throw_constraint_handler_s(
			            "strncat_s : s1 will overflow", ret);
			} else {
				lim = o;
				if (lim > 0) {
					s1e = (uintptr_t) s1 + m;
					s2e = (uintptr_t) s2 + o;
					if (((uintptr_t) s1 >=
					     (uintptr_t) s2 &&
					     (uintptr_t) s1 < s2e) ||
					    ((uintptr_t) s2 >=
					     (uintptr_t) s1 &&
					     (uintptr_t) s2 < s1e)) {
						zero_fill = 1;
						__throw_constraint_handler_s("strncat_s : memory overlaps", ret);
					} else {
						strncat(s1, s2, lim);
						ret = 0;
					}
				} else {
					/* trivial noop */
					ret = 0;
				}
			}
		}
	}
	/* do not zero fill on overlapping starts */
	if (ret && zero_fill && s1 != s2)
		s1[0] = 0;
	return (ret);
}
