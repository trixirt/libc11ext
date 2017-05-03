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
static const char *restrict m;

void h(const char *restrict msg, __attribute__((unused)) void *restrict ptr,
       errno_t error) {
  e = error;
  m = msg;
}

/* s1 null ptr */
ATF_TC_WITHOUT_HEAD(s1_null_ptr);
ATF_TC_BODY(s1_null_ptr, tc) {
  char b[2] = {5, 0};

  assert(strncat_s(0, 1, &b[0], 2) != 0);
  assert(b[0] == 5);
}

/* s1max > rmax */
ATF_TC_WITHOUT_HEAD(s1max_gt_rmax);
ATF_TC_BODY(s1max_gt_rmax, tc) {
  char a[2] = {3, 0};
  char b[2] = {5, 0};

  assert(strncat_s(&a[0], RSIZE_MAX + 1, &b[0], 2) != 0);
  assert(a[0] == 3);
  assert(a[1] == 0);
  assert(b[0] == 5);
}

/* s1max < 0 */
ATF_TC_WITHOUT_HEAD(s1max_lt_0);
ATF_TC_BODY(s1max_lt_0, tc) {
  char a[2] = {3, 0};
  const char b[2] = {5, 0};

  assert(strncat_s(&a[0], -1, &b[0], 2) != 0);
  assert(a[0] == 3);
  assert(a[1] == 0);
  assert(b[0] == 5);
}

/* normal */
ATF_TC_WITHOUT_HEAD(normal);
ATF_TC_BODY(normal, tc) {
  char a[4] = {4, 0, 2, 1};
  char b[2] = {5, 0};

  assert(strncat_s(&a[0], 4, &b[0], 2) == 0);
  assert(a[0] == 4);
  assert(a[1] == 5);
  assert(a[2] == 0);
  assert(b[0] == 5);
}

/* s2 null ptr */
ATF_TC_WITHOUT_HEAD(s2_null_ptr);
ATF_TC_BODY(s2_null_ptr, tc) {
  char a[4] = {4, 3, 2, 1};

  assert(strncat_s(&a[0], 4, 0, 1) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
}

/* s1 full already, poor s1 */
ATF_TC_WITHOUT_HEAD(s1_full_poor_s1);
ATF_TC_BODY(s1_full_poor_s1, tc) {
  char a[4] = {4, 3, 2, 1};
  char b[2] = {5, 0};

  assert(strncat_s(&a[0], 4, &b[0], 2) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
}

/* s1 full already, normal case */
ATF_TC_WITHOUT_HEAD(s1_full_normal_case);
ATF_TC_BODY(s1_full_normal_case, tc) {
  char a[4] = {4, 3, 2, 0};
  char b[2] = {5, 0};

  assert(strncat_s(&a[0], 4, &b[0], 2) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
}

/* s1 full already, but nothing to cat */
ATF_TC_WITHOUT_HEAD(s1_full_nothing_to_cat);
ATF_TC_BODY(s1_full_nothing_to_cat, tc) {
  char a[4] = {4, 3, 2, 0};
  char b[2] = {0, 1};

  assert(strncat_s(&a[0], 4, &b[0], 2) == 0);
  assert(a[0] == 4);
  assert(a[3] == 0);
  assert(b[0] == 0);
}

/* s1 will become full, not enough space for null terminator */
ATF_TC_WITHOUT_HEAD(s1_full_no_space_for_null);
ATF_TC_BODY(s1_full_no_space_for_null, tc) {
  char a[4] = {4, 3, 0, 1};
  char b[3] = {5, 6, 0};

  assert(strncat_s(&a[0], 4, &b[0], 3) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
}

/* s1 will become full, simple case */
ATF_TC_WITHOUT_HEAD(s1_becomes_full_simple);
ATF_TC_BODY(s1_becomes_full_simple, tc) {
  char a[6] = {4, 3, 0, 1, 2, 3};
  char b[5] = {5, 6, 7, 8, 0};

  assert(strncat_s(&a[0], 6, &b[0], 5) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
}

/* s1max == 0 */
ATF_TC_WITHOUT_HEAD(s1max_eq_0);
ATF_TC_BODY(s1max_eq_0, tc) {
  char a[2] = {3, 0};
  char b[2] = {5, 0};

  assert(strncat_s(&a[0], 0, &b[0], 2) != 0);
  assert(a[0] == 3);
  assert(a[1] == 0);
  assert(b[0] == 5);
}

/*
 * s1 will become full,
 * not enough space for null terminator,
 * overlapping s1 and s2
 */
ATF_TC_WITHOUT_HEAD(complex_1);
ATF_TC_BODY(complex_1, tc) {
  char a[4] = {4, 3, 0, 1};

  assert(strncat_s(&a[0], 4, &a[0], 4) != 0);
  assert(a[0] == 4);
  assert(a[1] == 3);
}

/* overlapping no zero fill */
ATF_TC_WITHOUT_HEAD(overlapping_no_zero_fill);
ATF_TC_BODY(overlapping_no_zero_fill, tc) {
  char a[5] = {4, 3, 0, 1, 0};

  assert(strncat_s(&a[0], 5, &a[0], 5) != 0);
  assert(a[0] == 4);
  assert(a[1] == 3);
}

/* overlapping zero fill */
ATF_TC_WITHOUT_HEAD(overlap_zero_fill);
ATF_TC_BODY(overlap_zero_fill, tc) {
  char a[5] = {4, 3, 0, 1, 0};

  assert(strncat_s(&a[0], 5, &a[1], 4) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
}

/* overlapping, nothing to cat */
ATF_TC_WITHOUT_HEAD(overlapping_nothing_to_cat);
ATF_TC_BODY(overlapping_nothing_to_cat, tc) {
  char a[5] = {4, 3, 0, 1, 0};

  assert(strncat_s(&a[0], 5, &a[2], 3) == 0);
  assert(a[0] == 4);
  assert(a[2] == 0);
}

/* s1 null ptr, handler */
ATF_TC_WITHOUT_HEAD(s1_null_ptr_handler);
ATF_TC_BODY(s1_null_ptr_handler, tc) {
  char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(0, 1, &b[0], 2) != 0);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 is NULL"));
}

/* s1max > rmax, handler */
ATF_TC_WITHOUT_HEAD(s1max_gt_rmax_handler);
ATF_TC_BODY(s1max_gt_rmax_handler, tc) {
  char a[2] = {3, 0};
  char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], RSIZE_MAX + 1, &b[0], 2) != 0);
  assert(a[0] == 3);
  assert(a[1] == 0);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1max > RSIZE_MAX"));
}

/* s1max < 0, handler */
ATF_TC_WITHOUT_HEAD(s1max_lt_0_handler);
ATF_TC_BODY(s1max_lt_0_handler, tc) {
  char a[2] = {3, 0};
  const char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], -1, &b[0], 2) != 0);
  assert(a[0] == 3);
  assert(a[1] == 0);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1max > RSIZE_MAX"));
}

/* normal, handler */
ATF_TC_WITHOUT_HEAD(normal_handler);
ATF_TC_BODY(normal_handler, tc) {
  char a[4] = {4, 0, 2, 1};
  char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &b[0], 2) == 0);
  assert(a[0] == 4);
  assert(a[1] == 5);
  assert(a[2] == 0);
  assert(b[0] == 5);
  assert(e == 0);
  assert(m == NULL);
}

/* s2 null ptr, handler */
ATF_TC_WITHOUT_HEAD(s2_null_ptr_handler);
ATF_TC_BODY(s2_null_ptr_handler, tc) {
  char a[4] = {4, 3, 2, 1};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, 0, 1) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s2 is NULL"));
}

/* s1 full already, poor s1, handler */
ATF_TC_WITHOUT_HEAD(s1_full_poor_s1_handler);
ATF_TC_BODY(s1_full_poor_s1_handler, tc) {
  char a[4] = {4, 3, 2, 1};
  char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &b[0], 2) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 is full"));
}

/* s1 full already, normal case, handler */
ATF_TC_WITHOUT_HEAD(s1_full_normal_handler);
ATF_TC_BODY(s1_full_normal_handler, tc) {
  char a[4] = {4, 3, 2, 0};
  char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &b[0], 2) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 will overflow"));
}

/* s1 full already, but nothing to cat, handler */
ATF_TC_WITHOUT_HEAD(s1_full_nothing_to_cat_handler);
ATF_TC_BODY(s1_full_nothing_to_cat_handler, tc) {
  char a[4] = {4, 3, 2, 0};
  char b[2] = {0, 1};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &b[0], 2) == 0);
  assert(a[0] == 4);
  assert(a[3] == 0);
  assert(b[0] == 0);
  assert(e == 0);
  assert(m == NULL);
}

/* s1 will become full, not enough space for null terminator, handler */
ATF_TC_WITHOUT_HEAD(complex_2);
ATF_TC_BODY(complex_2, tc) {
  char a[4] = {4, 3, 0, 1};
  char b[3] = {5, 6, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &b[0], 3) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 will overflow"));
}

/* s1 will become full, simple case, handler */
ATF_TC_WITHOUT_HEAD(s1_becomes_full_simple_handler);
ATF_TC_BODY(s1_becomes_full_simple_handler, tc) {
  char a[6] = {4, 3, 0, 1, 2, 3};
  char b[5] = {5, 6, 7, 8, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 6, &b[0], 5) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 will overflow"));
}

/* s1max == 0, handler */
ATF_TC_WITHOUT_HEAD(s1max_eq_0_handler);
ATF_TC_BODY(s1max_eq_0_handler, tc) {
  char a[2] = {3, 0};
  char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 0, &b[0], 2) != 0);
  assert(a[0] == 3);
  assert(a[1] == 0);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1max == 0"));
}

/*
 * s1 will become full,
 * not enough space for null terminator,
 * overlapping s1 and s2,
 * handler
 */
ATF_TC_WITHOUT_HEAD(complex_3);
ATF_TC_BODY(complex_3, tc) {
  char a[4] = {4, 3, 0, 1};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &a[0], 4) != 0);
  assert(a[0] == 4);
  assert(a[1] == 3);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 will overflow"));
}

/* overlapping no zero fill, handler */
ATF_TC_WITHOUT_HEAD(overlapping_no_zero_fill_handler);
ATF_TC_BODY(overlapping_no_zero_fill_handler, tc) {
  char a[5] = {4, 3, 0, 1, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 5, &a[0], 5) != 0);
  assert(a[0] == 4);
  assert(a[1] == 3);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : memory overlaps"));
}

/* overlapping zero fill, handler */
ATF_TC_WITHOUT_HEAD(overlapping_zero_fill_handler);
ATF_TC_BODY(overlapping_zero_fill_handler, tc) {
  char a[5] = {4, 3, 0, 1, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 5, &a[1], 4) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : memory overlaps"));
}

/* n > rmax */
ATF_TC_WITHOUT_HEAD(n_gt_rmax);
ATF_TC_BODY(n_gt_rmax, tc) {
  char a[2] = {3, 0};
  char b[2] = {5, 0};

  assert(strncat_s(&a[0], 2, &b[0], RSIZE_MAX + 1) != 0);
  assert(a[0] == 0);
  assert(a[1] == 0);
  assert(b[0] == 5);
}

/* n < 0 */
ATF_TC_WITHOUT_HEAD(n_lt_0);
ATF_TC_BODY(n_lt_0, tc) {
  char a[2] = {3, 0};
  const char b[2] = {5, 0};

  assert(strncat_s(&a[0], 2, &b[0], -1) != 0);
  assert(a[0] == 0);
  assert(a[1] == 0);
  assert(b[0] == 5);
}

/* n > rmax, handler */
ATF_TC_WITHOUT_HEAD(n_gt_rmax_handler);
ATF_TC_BODY(n_gt_rmax_handler, tc) {
  char a[2] = {3, 0};
  char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 2, &b[0], RSIZE_MAX + 1) != 0);
  assert(a[0] == 0);
  assert(a[1] == 0);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : n > RSIZE_MAX"));
}

/* n < 0, handler */
ATF_TC_WITHOUT_HEAD(n_lt_0_handler);
ATF_TC_BODY(n_lt_0_handler, tc) {
  char a[2] = {3, 0};
  const char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 2, &b[0], -1) != 0);
  assert(a[0] == 0);
  assert(a[1] == 0);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : n > RSIZE_MAX"));
}

/* overlapping, nothing to cat, handler */
ATF_TC_WITHOUT_HEAD(overlapping_nothing_to_cat_handler);
ATF_TC_BODY(overlapping_nothing_to_cat_handler, tc) {
  char a[5] = {4, 3, 0, 1, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 5, &a[2], 1) == 0);
  assert(a[0] == 4);
  assert(a[2] == 0);
  assert(e == 0);
  assert(m == NULL);
}

/* normal, n */
ATF_TC_WITHOUT_HEAD(normal_n);
ATF_TC_BODY(normal_n, tc) {
  char a[4] = {4, 0, 2, 1};
  char b[2] = {5, 0};

  assert(strncat_s(&a[0], 4, &b[0], 1) == 0);
  assert(a[0] == 4);
  assert(a[1] == 5);
  assert(a[2] == 0);
  assert(b[0] == 5);
}

/* s1 full already, normal case, n, handler */
ATF_TC_WITHOUT_HEAD(s1_full_normal_n_handler);
ATF_TC_BODY(s1_full_normal_n_handler, tc) {
  char a[4] = {4, 3, 2, 0};
  char b[2] = {5, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &b[0], 1) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 will overflow"));
}

/* s1 will become full, n, simple case, handler */
ATF_TC_WITHOUT_HEAD(s1_will_become_full_n_simple_handler);
ATF_TC_BODY(s1_will_become_full_n_simple_handler, tc) {
  char a[6] = {4, 3, 0, 1, 2, 3};
  char b[5] = {5, 6, 7, 8, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 6, &b[0], 4) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 will overflow"));
}

/* s1 full already, but nothing to cat, n, handler */
ATF_TC_WITHOUT_HEAD(s1_full_nothing_to_cat_n_handler);
ATF_TC_BODY(s1_full_nothing_to_cat_n_handler, tc) {
  char a[4] = {4, 3, 2, 0};
  char b[2] = {0, 1};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &b[0], 0) == 0);
  assert(a[0] == 4);
  assert(a[3] == 0);
  assert(b[0] == 0);
  assert(e == 0);
  assert(m == NULL);
}

/* s1 will become full, not enough space for null terminator, n, handler */
ATF_TC_WITHOUT_HEAD(complex_4);
ATF_TC_BODY(complex_4, tc) {
  char a[4] = {4, 3, 0, 1};
  char b[3] = {5, 6, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 4, &b[0], 2) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 will overflow"));
}

/* s1 will become full, simple case, n, handler */
ATF_TC_WITHOUT_HEAD(s1_will_become_full_simple_n_handler);
ATF_TC_BODY(s1_will_become_full_simple_n_handler, tc) {
  char a[6] = {4, 3, 0, 1, 2, 3};
  char b[5] = {5, 6, 7, 8, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 6, &b[0], 4) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(b[0] == 5);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : s1 will overflow"));
}

/* overlapping no zero fill, n, handler */
ATF_TC_WITHOUT_HEAD(overlapping_no_zero_fill_n_handler);
ATF_TC_BODY(overlapping_no_zero_fill_n_handler, tc) {
  char a[5] = {4, 3, 0, 1, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 5, &a[0], 2) != 0);
  assert(a[0] == 4);
  assert(a[1] == 3);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : memory overlaps"));
}

/* overlapping zero fill, n, handler */
ATF_TC_WITHOUT_HEAD(overlapping_zero_fill_n_handler);
ATF_TC_BODY(overlapping_zero_fill_n_handler, tc) {
  char a[5] = {4, 3, 0, 1, 0};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strncat_s(&a[0], 5, &a[1], 1) != 0);
  assert(a[0] == 0);
  assert(a[1] == 3);
  assert(e != 0);
  assert(0 == strcmp(m, "strncat_s : memory overlaps"));
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, s1_null_ptr);
  ATF_TP_ADD_TC(tp, s1max_gt_rmax);
  ATF_TP_ADD_TC(tp, s1max_lt_0);
  ATF_TP_ADD_TC(tp, normal);
  ATF_TP_ADD_TC(tp, s2_null_ptr);
  ATF_TP_ADD_TC(tp, s1_full_poor_s1);
  ATF_TP_ADD_TC(tp, s1_full_normal_case);
  ATF_TP_ADD_TC(tp, s1_full_nothing_to_cat);
  ATF_TP_ADD_TC(tp, s1_full_no_space_for_null);
  ATF_TP_ADD_TC(tp, s1_becomes_full_simple);
  ATF_TP_ADD_TC(tp, s1max_eq_0);
  ATF_TP_ADD_TC(tp, complex_1);
  ATF_TP_ADD_TC(tp, overlapping_no_zero_fill);
  ATF_TP_ADD_TC(tp, overlap_zero_fill);
  ATF_TP_ADD_TC(tp, overlapping_nothing_to_cat);
  ATF_TP_ADD_TC(tp, s1_null_ptr_handler);
  ATF_TP_ADD_TC(tp, s1max_gt_rmax_handler);
  ATF_TP_ADD_TC(tp, s1max_lt_0_handler);
  ATF_TP_ADD_TC(tp, normal_handler);
  ATF_TP_ADD_TC(tp, s2_null_ptr_handler);
  ATF_TP_ADD_TC(tp, s1_full_poor_s1_handler);
  ATF_TP_ADD_TC(tp, s1_full_normal_handler);
  ATF_TP_ADD_TC(tp, s1_full_nothing_to_cat_handler);
  ATF_TP_ADD_TC(tp, complex_2);
  ATF_TP_ADD_TC(tp, s1_becomes_full_simple_handler);
  ATF_TP_ADD_TC(tp, s1max_eq_0_handler);
  ATF_TP_ADD_TC(tp, complex_3);
  ATF_TP_ADD_TC(tp, overlapping_no_zero_fill_handler);
  ATF_TP_ADD_TC(tp, overlapping_zero_fill_handler);
  ATF_TP_ADD_TC(tp, overlapping_nothing_to_cat_handler);
  ATF_TP_ADD_TC(tp, n_gt_rmax);
  ATF_TP_ADD_TC(tp, n_lt_0);
  ATF_TP_ADD_TC(tp, n_gt_rmax_handler);
  ATF_TP_ADD_TC(tp, n_lt_0_handler);
  ATF_TP_ADD_TC(tp, normal_n);
  ATF_TP_ADD_TC(tp, s1_full_normal_n_handler);
  ATF_TP_ADD_TC(tp, s1_will_become_full_n_simple_handler);
  ATF_TP_ADD_TC(tp, s1_full_nothing_to_cat_n_handler);
  ATF_TP_ADD_TC(tp, complex_4);
  ATF_TP_ADD_TC(tp, s1_will_become_full_simple_n_handler);
  ATF_TP_ADD_TC(tp, overlapping_no_zero_fill_n_handler);
  ATF_TP_ADD_TC(tp, overlapping_zero_fill_n_handler);
  return (atf_no_error());
}
