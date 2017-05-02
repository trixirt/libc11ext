/*-
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

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <atf-c.h>

static errno_t e;
static const char * restrict m;

void
h(const char * restrict msg, void * restrict ptr __unused, errno_t error)
{
	e = error;
	m = msg;
}

/* s1 null ptr */
ATF_TC_WITHOUT_HEAD(s1_null_ptr);
ATF_TC_BODY(s1_null_ptr, tc)
{
	char b;

	b = 5;
	assert(memmove_s(0, 1, &b, 1) != 0);
	assert(b == 5);
}

/* s1max > rmax */
ATF_TC_WITHOUT_HEAD(s1max_gt_rmax);
ATF_TC_BODY(s1max_gt_rmax, tc)
{
	char a, b;

	a = 3;
	b = 5;
	assert(memmove_s(&b, RSIZE_MAX + 1, &b, 1) != 0);
	assert(a == 3);
	assert(b == 5);
}

/* s1max < 0 */
ATF_TC_WITHOUT_HEAD(s1max_lt_0);
ATF_TC_BODY(s1max_lt_0, tc)
{
	char a, b;

	a = 3;
	b = 5;
	assert(memmove_s(&b, -1, &b, 1) != 0);
	assert(a == 3);
	assert(b == 5);
}

/* normal */
ATF_TC_WITHOUT_HEAD(normal);
ATF_TC_BODY(normal, tc)
{
	char a, b;

	a = 3;
	b = 5;
	assert(memmove_s(&a, 1, &b, 1) == 0);
	assert(a == 5);
	assert(b == 5);
}

/* s2 null ptr */
ATF_TC_WITHOUT_HEAD(s2_null_ptr);
ATF_TC_BODY(s2_null_ptr, tc)
{
	char a;

	a = 5;
	assert(memmove_s(&a, 1, 0, 1) != 0);
	assert(a == 0);
}

/* n > rmax */
ATF_TC_WITHOUT_HEAD(n_gt_rmax);
ATF_TC_BODY(n_gt_rmax, tc)
{
	char a, b;

	a = 3;
	b = 5;
	assert(memmove_s(&a, 1, &b, RSIZE_MAX + 1) != 0);
	assert(a == 0);
	assert(b == 5);
}

/* n < 0 */
ATF_TC_WITHOUT_HEAD(n_lt_0);
ATF_TC_BODY(n_lt_0, tc)
{
	char a, b;

	a = 3;
	b = 5;
	assert(memmove_s(&a, 1, &b, -1) != 0);
	assert(a == 0);
	assert(b == 5);
}

/* n < s1max */
ATF_TC_WITHOUT_HEAD(n_lt_s1max);
ATF_TC_BODY(n_lt_s1max, tc)
{
	char a[3] = { 1, 2, 3 };
	char b[3] = { 5, 6, 7 };
	assert(memmove_s(&a[0], 3, &b[0], 2) == 0);
	assert(a[0] == 5);
	assert(a[1] == 6);
	assert(a[2] == 3);
}

/* n > s1max */
ATF_TC_WITHOUT_HEAD(n_gt_s1max);
ATF_TC_BODY(n_gt_s1max, tc)
{
	char a[3] = { 1, 2, 3 };
	char b[3] = { 5, 6, 7 };

	assert(memmove_s(&a[0], 2, &b[0], 3) == 0);
	assert(a[0] == 5);
	assert(a[1] == 6);
	assert(a[2] == 3);
}

/* null ptr, handler */
ATF_TC_WITHOUT_HEAD(s1_null_ptr_handler);
ATF_TC_BODY(s1_null_ptr_handler, tc)
{
	char b;

	b = 5;
	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(memmove_s(0, 1, &b, 1) != 0);
	assert(e != 0);
	assert(0 == strcmp(m, "memmove_s : s1 is NULL"));
}

/* s1max > rmax, handler */
ATF_TC_WITHOUT_HEAD(s1max_gt_rmax_handler);
ATF_TC_BODY(s1max_gt_rmax_handler, tc)
{
	char a, b;

	a = 3;
	b = 5;
	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(memmove_s(&b, RSIZE_MAX + 1, &b, 1) != 0);
	assert(a == 3);
	assert(b == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "memmove_s : s1max > RSIZE_MAX"));
}

/* s1max < 0, handler */
ATF_TC_WITHOUT_HEAD(s1max_lt_0_handler);
ATF_TC_BODY(s1max_lt_0_handler, tc)
{
	char a, b;

	a = 3;
	b = 5;
	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(memmove_s(&b, -1, &b, 1) != 0);
	assert(a == 3);
	assert(b == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "memmove_s : s1max > RSIZE_MAX"));
}

/* normal, handler */
ATF_TC_WITHOUT_HEAD(normal_handler);
ATF_TC_BODY(normal_handler, tc)
{
	char a, b;

	a = 3;
	b = 5;
	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(memmove_s(&a, 1, &b, 1) == 0);
	assert(a == 5);
	assert(b == 5);
	assert(e == 0);
	assert(m == NULL);
}

/* s2 null ptr, handler */
ATF_TC_WITHOUT_HEAD(s2_null_ptr_handler);
ATF_TC_BODY(s2_null_ptr_handler, tc)
{
	char a;

	a = 5;
	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(memmove_s(&a, 1, 0, 1) != 0);
	assert(a == 0);
	assert(e != 0);
	assert(0 == strcmp(m, "memmove_s : s2 is NULL"));
}

/* n > rmax, handler */
ATF_TC_WITHOUT_HEAD(n_gt_rmax_handler);
ATF_TC_BODY(n_gt_rmax_handler, tc)
{
	char a, b;

	a = 3;
	b = 5;
	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(memmove_s(&a, 1, &b, RSIZE_MAX + 1) != 0);
	assert(a == 0);
	assert(b == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "memmove_s : n > RSIZE_MAX"));
}

/* n < 0, handler */
ATF_TC_WITHOUT_HEAD(n_lt_0_handler);
ATF_TC_BODY(n_lt_0_handler, tc)
{
	char a, b;

	a = 3;
	b = 5;
	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(memmove_s(&a, 1, &b, -1) != 0);
	assert(a == 0);
	assert(b == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "memmove_s : n > RSIZE_MAX"));
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, s1_null_ptr);
	ATF_TP_ADD_TC(tp, s1max_gt_rmax);
	ATF_TP_ADD_TC(tp, s1max_lt_0);
	ATF_TP_ADD_TC(tp, normal);
	ATF_TP_ADD_TC(tp, s2_null_ptr);
	ATF_TP_ADD_TC(tp, n_gt_rmax);
	ATF_TP_ADD_TC(tp, n_lt_0);
	ATF_TP_ADD_TC(tp, n_lt_s1max);
	ATF_TP_ADD_TC(tp, n_gt_s1max);
	ATF_TP_ADD_TC(tp, s1_null_ptr_handler);
	ATF_TP_ADD_TC(tp, s1max_gt_rmax_handler);
	ATF_TP_ADD_TC(tp, s1max_lt_0_handler);
	ATF_TP_ADD_TC(tp, normal_handler);
	ATF_TP_ADD_TC(tp, s2_null_ptr_handler);
	ATF_TP_ADD_TC(tp, n_gt_rmax_handler);
	ATF_TP_ADD_TC(tp, n_lt_0_handler);
	return (atf_no_error());
}
