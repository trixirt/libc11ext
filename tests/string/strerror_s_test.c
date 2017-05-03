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
#include <errno.h>
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

/* s null ptr */
ATF_TC_WITHOUT_HEAD(s_null_ptr);
ATF_TC_BODY(s_null_ptr, tc) { assert(strerror_s(0, 1, EINVAL) != 0); }

/* s null ptr, handler */
ATF_TC_WITHOUT_HEAD(s_null_ptr_handler);
ATF_TC_BODY(s_null_ptr_handler, tc) {
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strerror_s(0, 1, EINVAL) != 0);
  assert(e != 0);
  assert(0 == strcmp(m, "strerror_s : s is NULL"));
}

/* maxsize > rmax */
ATF_TC_WITHOUT_HEAD(maxsize_gt_rmax);
ATF_TC_BODY(maxsize_gt_rmax, tc) {
  char a[2] = {1, 2};

  assert(strerror_s(&a[0], RSIZE_MAX + 1, EINVAL) != 0);
  assert(a[0] == 1);
  assert(a[1] == 2);
}

/* maxsize > rmax, handler */
ATF_TC_WITHOUT_HEAD(maxsize_gt_rmax_handler);
ATF_TC_BODY(maxsize_gt_rmax_handler, tc) {
  char a[2] = {1, 2};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strerror_s(&a[0], RSIZE_MAX + 1, EINVAL) != 0);
  assert(a[0] == 1);
  assert(a[1] == 2);
  assert(e != 0);
  assert(0 == strcmp(m, "strerror_s : maxsize > RSIZE_MAX"));
}

/* maxsize < 0 */
ATF_TC_WITHOUT_HEAD(maxsize_lt_0);
ATF_TC_BODY(maxsize_lt_0, tc) {
  char a[2] = {1, 2};

  assert(strerror_s(&a[0], -1, EINVAL) != 0);
  assert(a[0] == 1);
  assert(a[1] == 2);
}

/* maxsize < 0, handler */
ATF_TC_WITHOUT_HEAD(maxsize_lt_0_handler);
ATF_TC_BODY(maxsize_lt_0_handler, tc) {
  char a[2] = {1, 2};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strerror_s(&a[0], -1, EINVAL) != 0);
  assert(a[0] == 1);
  assert(a[1] == 2);
  assert(e != 0);
  assert(0 == strcmp(m, "strerror_s : maxsize > RSIZE_MAX"));
}

/* maxsize = 0 */
ATF_TC_WITHOUT_HEAD(maxsize_eq_0);
ATF_TC_BODY(maxsize_eq_0, tc) {
  char a[2] = {1, 2};

  assert(strerror_s(&a[0], 0, EINVAL) != 0);
  assert(a[0] == 1);
  assert(a[1] == 2);
}

/* maxsize = 0, handler */
ATF_TC_WITHOUT_HEAD(maxsize_eq_0_handler);
ATF_TC_BODY(maxsize_eq_0_handler, tc) {
  char a[2] = {1, 2};

  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(strerror_s(&a[0], 0, EINVAL) != 0);
  assert(a[0] == 1);
  assert(a[1] == 2);
  assert(e != 0);
  assert(0 == strcmp(m, "strerror_s : maxsize == 0"));
}

/* short 1 */
ATF_TC_WITHOUT_HEAD(short_1);
ATF_TC_BODY(short_1, tc) {
  char a[5] = {1, 2, 3, 4, 5};

  assert(strerror_s(&a[0], 1, EINVAL) != 0);
  assert(a[0] == 0);
  assert(a[1] == 2);
}

/* short 2 */
ATF_TC_WITHOUT_HEAD(short_2);
ATF_TC_BODY(short_2, tc) {
  char a[5] = {1, 2, 3, 4, 5};

  assert(strerror_s(&a[0], 2, EINVAL) != 0);
  assert(a[0] == '.');
  assert(a[1] == 0);
  assert(a[2] == 3);
}

/* short 3 */
ATF_TC_WITHOUT_HEAD(short_3);
ATF_TC_BODY(short_3, tc) {
  char a[5] = {1, 2, 3, 4, 5};

  assert(strerror_s(&a[0], 3, EINVAL) != 0);
  assert(a[0] == '.');
  assert(a[1] == '.');
  assert(a[2] == 0);
  assert(a[3] == 4);
}

/* short 4 */
ATF_TC_WITHOUT_HEAD(short_4);
ATF_TC_BODY(short_4, tc) {
  char a[5] = {1, 2, 3, 4, 5};

  assert(strerror_s(&a[0], 4, EINVAL) != 0);
  assert(a[0] == '.');
  assert(a[1] == '.');
  assert(a[2] == '.');
  assert(a[3] == 0);
  assert(a[4] == 5);
}

/* normal */
ATF_TC_WITHOUT_HEAD(normal);
ATF_TC_BODY(normal, tc) {
  char *a;
  size_t len;

  len = strerrorlen_s(EINVAL) + 1;
  a = (char *)calloc(len, 1);
  a[len - 1] = 'f';
  assert(strerror_s(a, len, EINVAL) == 0);
  assert(a[0] != 0);
  assert(a[len - 1] == 0);
}

/* normal, handler */
ATF_TC_WITHOUT_HEAD(normal_handler);
ATF_TC_BODY(normal_handler, tc) {
  char *a;
  size_t len;

  len = strerrorlen_s(EINVAL) + 1;
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  a = (char *)calloc(len, 1);
  a[len - 1] = 'f';
  assert(strerror_s(a, len, EINVAL) == 0);
  assert(a[0] != 0);
  assert(a[len - 1] == 0);
  assert(e == 0);
  assert(m == NULL);
}

/* mid short */
ATF_TC_WITHOUT_HEAD(mid_short);
ATF_TC_BODY(mid_short, tc) {
  char *a;
  size_t len;

  len = strerrorlen_s(EINVAL) + 1;
  if (len > 5) {
    a = (char *)malloc(len);
    memset(a, -1, len);
    assert(strerror_s(a, len - 1, EINVAL) != 0);
    assert(a[0] != -1);
    assert(a[0] != 0);
    assert(a[len - 1] == -1);
    assert(a[len - 2] == 0);
    assert(a[len - 3] == '.');
    assert(a[len - 4] == '.');
    assert(a[len - 5] == '.');
  }
}

/* abusive normal, handler */
ATF_TC_WITHOUT_HEAD(abusive_normal_handler);
ATF_TC_BODY(abusive_normal_handler, tc) {
  int i;
  char *a;
  size_t len;

  for (i = -1234; i < 1235; i++) {
    len = strerrorlen_s(i) + 1;
    e = 0;
    m = NULL;
    set_constraint_handler_s(h);
    a = (char *)calloc(len, 1);
    a[len - 1] = 'f';
    assert(strerror_s(a, len, i) == 0);
    assert(a[0] != 0);
    assert(a[len - 1] == 0);
    assert(e == 0);
    assert(m == NULL);
    free(a);
  }
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, s_null_ptr);
  ATF_TP_ADD_TC(tp, s_null_ptr_handler);
  ATF_TP_ADD_TC(tp, maxsize_gt_rmax);
  ATF_TP_ADD_TC(tp, maxsize_gt_rmax_handler);
  ATF_TP_ADD_TC(tp, maxsize_lt_0);
  ATF_TP_ADD_TC(tp, maxsize_lt_0_handler);
  ATF_TP_ADD_TC(tp, maxsize_eq_0);
  ATF_TP_ADD_TC(tp, maxsize_eq_0_handler);
  ATF_TP_ADD_TC(tp, short_1);
  ATF_TP_ADD_TC(tp, short_2);
  ATF_TP_ADD_TC(tp, short_3);
  ATF_TP_ADD_TC(tp, short_4);
  ATF_TP_ADD_TC(tp, normal);
  ATF_TP_ADD_TC(tp, normal_handler);
  ATF_TP_ADD_TC(tp, mid_short);
  ATF_TP_ADD_TC(tp, abusive_normal_handler);
  return (atf_no_error());
}
