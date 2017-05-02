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

static int _check_first(char a, const char *b, size_t n)
{
	int ret;
	size_t i;

	ret = 1;
	for (i = 0; i < n; i++) {
		if (a == b[i]) {
			ret = 0;
			break;
		}
	}
	return (ret);
}

static void _setptr_first(char *a, size_t i, size_t max __unused, char **b)
{
	*b = a + i;
}

static void _setret_first(char *cur, size_t i, char **ret)
{
	*ret = &cur[i];
}

static void _setmax_first(rsize_t *max, size_t i, size_t lim)
{
	*max = lim - i;
}

static int _check_next(char a, const char *b, size_t n)
{
	int ret;
	size_t i;

	ret = 0;
	for (i = 0; i < n; i++) {
		if (a == b[i]) {
			ret = 1;
			break;
		}
	}
	return (ret);
}

static void _setptr_next(char *a, size_t i, size_t max, char **b)
{
	a[i] = 0;
	if (i+1 < max)
		*b = &a[i + 1];
}

static void _setret_next(char *cur, size_t i __unused, char **ret)
{
	*ret = cur;
}

static void _setmax_next(rsize_t *max, size_t i, size_t lim)
{
	if (i < lim)
		*max = lim - i - 1;
	else
		*max = 0;
}

/* ISO/IEC 9899:2011 K.3.7.3.1 */
char *strtok_s(char * __restrict s1, rsize_t * __restrict s1max,
    const char * __restrict s2, char ** __restrict ptr)
{
	char *ret, *cur;
	rsize_t i, lim, s2len;
	int (*check)(char, const char *, size_t);
	void (*setptr)(char *, size_t, size_t, char **);
	void (*setret)(char *, size_t, char **);
	void (*setmax)(rsize_t *, size_t, size_t);

	ret = NULL;
	if (s1max == NULL) {
		__throw_constraint_handler_s("strtok_s : s1max is NULL",
		    EINVAL);
	} else if (s2 == NULL) {
		__throw_constraint_handler_s("strtok_s : s2 is NULL", EINVAL);
	} else if (ptr == NULL) {
		__throw_constraint_handler_s("strtok_s : ptr is NULL", EINVAL);
	} else if (s1 == NULL && *ptr == NULL) {
		__throw_constraint_handler_s("strtok_s : *ptr is NULL",
		    EINVAL);
	} else if (*s1max > RSIZE_MAX) {
		__throw_constraint_handler_s("strtok_s : *s1max > RSIZE_MAX",
		    EINVAL);
	} else {
		s2len = strlen(s2);
		if (s2len != 0) {
			if (s1 != NULL) {
				cur = s1;
				check = _check_first;
				setptr = _setptr_first;
				setret = _setret_first;
				setmax = _setmax_first;
			} else {
				cur = *ptr;
				check = _check_next;
				setptr = _setptr_next;
				setret = _setret_next;
				setmax = _setmax_next;
			}
			lim = *s1max;
			*ptr = NULL;
			for (i = 0; i < lim; i++) {
				if (check(cur[i], s2, s2len)) {
					setptr(cur, i, lim, ptr);
					break;
				}
			}
			if (i < lim)
				setret(cur, i, &ret);
			setmax(s1max, i, lim);
		}
	}
	return (ret);
}
