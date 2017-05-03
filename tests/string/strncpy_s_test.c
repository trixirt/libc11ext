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

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libext.h>

#include <atf-c.h>

static errno_t e;
static const char * restrict m;

void
h(const char * restrict msg, __attribute__((unused)) void * restrict ptr, errno_t error)
{
	e = error;
	m = msg;
}

/* s1 null ptr */
ATF_TC_WITHOUT_HEAD(s1_null_ptr);
ATF_TC_BODY(s1_null_ptr, tc)
{
	char b[2] = { 5, 0 };

	assert(strncpy_s(0, 1, &b[0], 2) != 0);
	assert(b[0] == 5);
}

/* s1max > rmax */
ATF_TC_WITHOUT_HEAD(s1max_gt_rmax);
ATF_TC_BODY(s1max_gt_rmax, tc)
{
	char a[2] = { 3, 0 };
	char b[2] = { 5, 0 };

	assert(strncpy_s(&a[0], RSIZE_MAX + 1, &b[0], 2) != 0);
	assert(a[0] == 3);
	assert(a[1] == 0);
	assert(b[0] == 5);
}

/* s1max < 0 */
ATF_TC_WITHOUT_HEAD(s1max_lt_0);
ATF_TC_BODY(s1max_lt_0, tc)
{
	char a[2] = { 3, 0 };
	const char b[2] = { 5, 0 };

	assert(strncpy_s(&a[0], -1, &b[0], 2) != 0);
	assert(a[0] == 3);
	assert(a[1] == 0);
	assert(b[0] == 5);
}

/* normal */
ATF_TC_WITHOUT_HEAD(normal);
ATF_TC_BODY(normal, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[2] = { 5, 0 };

	assert(strncpy_s(&a[0], 4, &b[0], 2) == 0);
	assert(a[0] == 5);
	assert(a[1] == 0);
	assert(b[0] == 5);
}

/* s2 null ptr */
ATF_TC_WITHOUT_HEAD(s2_null_ptr);
ATF_TC_BODY(s2_null_ptr, tc)
{
	char a[4] = { 4, 3, 2, 1 };

	assert(strncpy_s(&a[0], 4, 0, 0) != 0);
	assert(a[0] == 0);
	assert(a[1] == 3);
}

/* n > s1max */
ATF_TC_WITHOUT_HEAD(n_gt_s1max);
ATF_TC_BODY(n_gt_s1max, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[6] = { 5, 4, 3, 2, 1, 0 };

	assert(strncpy_s(&a[0], 4, &b[0], 6) != 0);
	assert(a[0] == 0);
	assert(b[0] == 5);
}

/* n == s1max */
ATF_TC_WITHOUT_HEAD(n_eq_s1max);
ATF_TC_BODY(n_eq_s1max, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[4] = { 5, 4, 3, 0 };

	assert(strncpy_s(&a[0], 4, &b[0], 4) == 0);
	assert(a[0] == 5);
	assert(a[1] == 4);
	assert(a[2] == 3);
	assert(a[3] == 0);
	assert(b[0] == 5);
}

/* n == s1max, inferred */
ATF_TC_WITHOUT_HEAD(n_eq_s1max_inferred);
ATF_TC_BODY(n_eq_s1max_inferred, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[6] = { 5, 4, 3, 0, 2, 3 };

	assert(strncpy_s(&a[0], 4, &b[0], 6) == 0);
	assert(a[0] == 5);
	assert(a[1] == 4);
	assert(a[2] == 3);
	assert(a[3] == 0);
	assert(b[0] == 5);
}

/* overlap end */
ATF_TC_WITHOUT_HEAD(overlap_end);
ATF_TC_BODY(overlap_end, tc)
{
	char a[10] = { 4, 3, 2, 1, 1, 2, 3, 4, 0 };

	assert(strncpy_s(&a[0], 5, &a[4], 6) != 0);
	assert(a[0] == 0);
}

/* overlap middle */
ATF_TC_WITHOUT_HEAD(overlap_middle);
ATF_TC_BODY(overlap_middle, tc)
{
	char a[8] = { 4, 3, 2, 1, 1, 2, 3, 0 };

	assert(strncpy_s(&a[0], 8, &a[2], 6) != 0);
	assert(a[0] == 0);
}

/* overlap beginning */
ATF_TC_WITHOUT_HEAD(overlap_beginning);
ATF_TC_BODY(overlap_beginning, tc)
{
	char a[7] = { 4, 0, 2, 1, 3, 1, 2 };

	assert(strncpy_s(&a[0], 7, &a[0], 7) != 0);
	assert(a[0] == 4);
}

/* s1 null ptr, handler */
ATF_TC_WITHOUT_HEAD(s1_null_ptr_handler);
ATF_TC_BODY(s1_null_ptr_handler, tc)
{
	char b[2] = { 5, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(0, 1, &b[0], 2) != 0);
	assert(b[0] == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : s1 is NULL"));
}

/* s1max > rmax, handler */
ATF_TC_WITHOUT_HEAD(s1max_gt_rmax_handler);
ATF_TC_BODY(s1max_gt_rmax_handler, tc)
{
	char a[2] = { 3, 0 };
	char b[2] = { 5, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], RSIZE_MAX + 1, &b[0], 2) != 0);
	assert(a[0] == 3);
	assert(a[1] == 0);
	assert(b[0] == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : s1max > RSIZE_MAX"));
}

/* s1max < 0, handler */
ATF_TC_WITHOUT_HEAD(s1max_lt_0_handler);
ATF_TC_BODY(s1max_lt_0_handler, tc)
{
	char a[2] = { 3, 0 };
	const char b[2] = { 5, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], -1, &b[0], 2) != 0);
	assert(a[0] == 3);
	assert(a[1] == 0);
	assert(b[0] == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : s1max > RSIZE_MAX"));
}

/* normal, handler */
ATF_TC_WITHOUT_HEAD(normal_handler);
ATF_TC_BODY(normal_handler, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	const char b[2] = { 5, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], 4, &b[0], 2) == 0);
	assert(a[0] == 5);
	assert(a[1] == 0);
	assert(b[0] == 5);
	assert(e == 0);
	assert(m == NULL);
}

/* s2 null ptr, handler */
ATF_TC_WITHOUT_HEAD(s2_null_ptr_handler);
ATF_TC_BODY(s2_null_ptr_handler, tc)
{
	char a[4] = { 4, 3, 2, 1 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], 4, 0, 0) != 0);
	assert(a[0] == 0);
	assert(a[1] == 3);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : s2 is NULL"));
}

/* n > s1max, handler */
ATF_TC_WITHOUT_HEAD(n_gt_s1max_handler);
ATF_TC_BODY(n_gt_s1max_handler, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[6] = { 5, 4, 3, 2, 1, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], 4, &b[0], 6) != 0);
	assert(a[0] == 0);
	assert(b[0] == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : s2 will be truncated"));
}

/* n == s1max, handler */
ATF_TC_WITHOUT_HEAD(n_eq_s1max_handler);
ATF_TC_BODY(n_eq_s1max_handler, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	const char b[5] = { 5, 4, 3, 2, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], 4, &b[0], 4) != 0);
	assert(a[0] == 0);
	assert(b[0] == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : s2 will be truncated"));
}

/* overlap end, handler */
ATF_TC_WITHOUT_HEAD(overlap_end_handler);
ATF_TC_BODY(overlap_end_handler, tc)
{
	char a[10] = { 4, 3, 2, 1, 1, 2, 3, 4, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], 5, &a[4], 6) != 0);
	assert(a[0] == 0);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : memory overlaps"));
}

/* overlap middle, handler */
ATF_TC_WITHOUT_HEAD(overlap_middle_handler);
ATF_TC_BODY(overlap_middle_handler, tc)
{
	char a[8] = { 4, 3, 2, 1, 1, 2, 3, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], 8, &a[2], 6) != 0);
	assert(a[0] == 0);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : memory overlaps"));
}

/* overlap beginning, handler */
ATF_TC_WITHOUT_HEAD(overlap_beginning_handler);
ATF_TC_BODY(overlap_beginning_handler, tc)
{
	char a[7] = { 4, 0, 2, 1, 3, 1, 2 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], 7, &a[0], 7) != 0);
	assert(a[0] == 4);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : memory overlaps"));
}

/* s1max == 0, handler */
ATF_TC_WITHOUT_HEAD(s1max_eq_0_handler);
ATF_TC_BODY(s1max_eq_0_handler, tc)
{
	char a[2] = { 3, 0 };
	const char b[2] = { 5, 0 };

	e = 0;
	m = NULL;
	set_constraint_handler_s(h);
	assert(strncpy_s(&a[0], 0, &b[0], 2) != 0);
	assert(a[0] == 3);
	assert(a[1] == 0);
	assert(b[0] == 5);
	assert(e != 0);
	assert(0 == strcmp(m, "strncpy_s : s1max == 0"));
}

/* trivial input */
ATF_TC_WITHOUT_HEAD(trivial_input);
ATF_TC_BODY(trivial_input, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[1] = { 0 };

	assert(strncpy_s(&a[0], 4, &b[0], 1) == 0);
	assert(a[0] == 0);
	assert(b[0] == 0);
}

/* trivial input, inferred */
ATF_TC_WITHOUT_HEAD(trivial_input_inferred);
ATF_TC_BODY(trivial_input_inferred, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[5] = { 0, 1, 2, 3, 4 };

	assert(strncpy_s(&a[0], 4, &b[0], 5) == 0);
	assert(a[0] == 0);
	assert(b[0] == 0);
}

/* null terminate */
ATF_TC_WITHOUT_HEAD(null_terminate);
ATF_TC_BODY(null_terminate, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[2] = { 1, 2 };

	assert(strncpy_s(&a[0], 4, &b[0], 2) == 0);
	assert(a[0] == 1);
	assert(a[1] == 2);
	assert(a[2] == 0);
	assert(b[0] == 1);
}

/* do nothing */
ATF_TC_WITHOUT_HEAD(do_nothing);
ATF_TC_BODY(do_nothing, tc)
{
	char a[4] = { 4, 3, 2, 1 };
	char b[2] = { 1, 2 };

	assert(strncpy_s(&a[0], 4, &b[0], 0) == 0);
	assert(a[0] == 0);
	assert(b[0] == 1);
}

ATF_TP_ADD_TCS(tp)
{
	ATF_TP_ADD_TC(tp, s1_null_ptr);
	ATF_TP_ADD_TC(tp, s1max_gt_rmax);
	ATF_TP_ADD_TC(tp, s1max_lt_0);
	ATF_TP_ADD_TC(tp, normal);
	ATF_TP_ADD_TC(tp, s2_null_ptr);
	ATF_TP_ADD_TC(tp, n_gt_s1max);
	ATF_TP_ADD_TC(tp, n_eq_s1max);
	ATF_TP_ADD_TC(tp, n_eq_s1max_inferred);
	ATF_TP_ADD_TC(tp, overlap_end);
	ATF_TP_ADD_TC(tp, overlap_middle);
	ATF_TP_ADD_TC(tp, overlap_beginning);
	ATF_TP_ADD_TC(tp, s1_null_ptr_handler);
	ATF_TP_ADD_TC(tp, s1max_gt_rmax_handler);
	ATF_TP_ADD_TC(tp, s1max_lt_0_handler);
	ATF_TP_ADD_TC(tp, normal_handler);
	ATF_TP_ADD_TC(tp, s2_null_ptr_handler);
	ATF_TP_ADD_TC(tp, n_gt_s1max_handler);
	ATF_TP_ADD_TC(tp, n_eq_s1max_handler);
	ATF_TP_ADD_TC(tp, overlap_end_handler);
	ATF_TP_ADD_TC(tp, overlap_middle_handler);
	ATF_TP_ADD_TC(tp, overlap_beginning_handler);
	ATF_TP_ADD_TC(tp, s1max_eq_0_handler);
	ATF_TP_ADD_TC(tp, trivial_input);
	ATF_TP_ADD_TC(tp, trivial_input_inferred);
	ATF_TP_ADD_TC(tp, null_terminate);
	ATF_TP_ADD_TC(tp, do_nothing);
	return (atf_no_error());
}
