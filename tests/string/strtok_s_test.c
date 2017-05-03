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

/* s1 & *ptr null */
ATF_TC_WITHOUT_HEAD(s1_and_ptr_null);
ATF_TC_BODY(s1_and_ptr_null, tc) {
  char *ptr;
  rsize_t s1max;

  ptr = NULL;
  s1max = 1;
  assert(NULL == strtok_s(0, &s1max, "", &ptr));
}

/* s1 & *ptr null, handler */
ATF_TC_WITHOUT_HEAD(s1_and_ptr_null_handler);
ATF_TC_BODY(s1_and_ptr_null_handler, tc) {
  char *ptr;
  rsize_t s1max;

  ptr = NULL;
  s1max = 1;
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(NULL == strtok_s(0, &s1max, "", &ptr));
  assert(e != 0);
  assert(0 == strcmp(m, "strtok_s : *ptr is NULL"));
}

/* s1max null */
ATF_TC_WITHOUT_HEAD(s1max_null);
ATF_TC_BODY(s1max_null, tc) {
  char *ptr;

  ptr = NULL;
  assert(NULL == strtok_s("", NULL, "", &ptr));
}

/* s1max null, handler */
ATF_TC_WITHOUT_HEAD(s1max_null_handler);
ATF_TC_BODY(s1max_null_handler, tc) {
  char *ptr;

  ptr = NULL;
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(NULL == strtok_s("", NULL, "", &ptr));
  assert(e != 0);
  assert(0 == strcmp(m, "strtok_s : s1max is NULL"));
}

/* s2 null */
ATF_TC_WITHOUT_HEAD(s2_null);
ATF_TC_BODY(s2_null, tc) {
  char *ptr;
  rsize_t s1max;

  ptr = NULL;
  s1max = 1;
  assert(NULL == strtok_s("", &s1max, NULL, &ptr));
}

/* s2 null, handler */
ATF_TC_WITHOUT_HEAD(s2_null_handler);
ATF_TC_BODY(s2_null_handler, tc) {
  char *ptr;
  rsize_t s1max;

  ptr = NULL;
  s1max = 1;
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(NULL == strtok_s("", &s1max, NULL, &ptr));
  assert(e != 0);
  assert(0 == strcmp(m, "strtok_s : s2 is NULL"));
}

/* ptr null */
ATF_TC_WITHOUT_HEAD(ptr_null);
ATF_TC_BODY(ptr_null, tc) {
  rsize_t s1max;

  s1max = 1;
  assert(NULL == strtok_s("", &s1max, "", NULL));
}

/* ptr null, handler */
ATF_TC_WITHOUT_HEAD(ptr_null_handler);
ATF_TC_BODY(ptr_null_handler, tc) {
  rsize_t s1max;

  s1max = 1;
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(NULL == strtok_s("", &s1max, "", NULL));
  assert(e != 0);
  assert(0 == strcmp(m, "strtok_s : ptr is NULL"));
}

/* s1max > RSIZE_MAX */
ATF_TC_WITHOUT_HEAD(s1max_gt_rsize_max);
ATF_TC_BODY(s1max_gt_rsize_max, tc) {
  char *ptr;
  rsize_t s1max;

  s1max = RSIZE_MAX + 1;
  assert(NULL == strtok_s("", &s1max, "", &ptr));
}

/* s1max > RSIZE_MAX, handler */
ATF_TC_WITHOUT_HEAD(s1max_gt_rsize_max_handler);
ATF_TC_BODY(s1max_gt_rsize_max_handler, tc) {
  char *ptr;
  rsize_t s1max;

  s1max = RSIZE_MAX + 1;
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(NULL == strtok_s("", &s1max, "", &ptr));
  assert(e != 0);
  assert(0 == strcmp(m, "strtok_s : *s1max > RSIZE_MAX"));
}

/* s1max < 0 */
ATF_TC_WITHOUT_HEAD(s1max_lt_0);
ATF_TC_BODY(s1max_lt_0, tc) {
  char *ptr;
  rsize_t s1max;

  s1max = -1;
  assert(NULL == strtok_s("", &s1max, "", &ptr));
}

/* s1max < 0, handler */
ATF_TC_WITHOUT_HEAD(s1max_lt_0_handler);
ATF_TC_BODY(s1max_lt_0_handler, tc) {
  char *ptr;
  rsize_t s1max;

  s1max = -1;
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(NULL == strtok_s("", &s1max, "", &ptr));
  assert(e != 0);
  assert(0 == strcmp(m, "strtok_s : *s1max > RSIZE_MAX"));
}

/* s2 len 0 */
ATF_TC_WITHOUT_HEAD(s2_len_eq_0);
ATF_TC_BODY(s2_len_eq_0, tc) {
  char *ptr;
  rsize_t s1max;

  s1max = 4;
  assert(NULL == strtok_s("abcd", &s1max, "", &ptr));
}

/* s2 len 0, handler */
ATF_TC_WITHOUT_HEAD(s2_len_eq_0_handler);
ATF_TC_BODY(s2_len_eq_0_handler, tc) {
  char *ptr;
  rsize_t s1max;

  s1max = 4;
  e = 0;
  m = NULL;
  set_constraint_handler_s(h);
  assert(NULL == strtok_s("abcd", &s1max, "", &ptr));
  assert(e == 0);
  assert(m == NULL);
}

/* first, failure */
ATF_TC_WITHOUT_HEAD(first_failure);
ATF_TC_BODY(first_failure, tc) {
  char *str = " + - + - ";
  char *dlim = " +-";
  char *ptr;
  rsize_t s1max;

  s1max = strlen(str);
  assert(NULL == strtok_s(str, &s1max, dlim, &ptr));
  assert(s1max == 0);
  assert(ptr == NULL);
}

/* next, failure */
ATF_TC_WITHOUT_HEAD(next_failure);
ATF_TC_BODY(next_failure, tc) {
  char *str = " + - + - ";
  char *dlim = "abc";
  char *ptr = str;
  rsize_t s1max;

  s1max = strlen(str);
  assert(NULL == strtok_s(NULL, &s1max, dlim, &ptr));
  assert(s1max == 0);
  assert(ptr == NULL);
}

/* first, success */
ATF_TC_WITHOUT_HEAD(first_success);
ATF_TC_BODY(first_success, tc) {
  char *str = " A - + - ";
  char *dlim = " +-";
  char *ptr;
  char *tok;
  rsize_t s1max;

  s1max = strlen(str);
  tok = strtok_s(str, &s1max, dlim, &ptr);
  assert(tok == &str[1]);
  assert(tok == ptr);
  assert(s1max == strlen(tok));
  assert(str[0] == ' ');
  assert(str[2] == ' ');
}

/* first, success 2 */
ATF_TC_WITHOUT_HEAD(first_success_2);
ATF_TC_BODY(first_success_2, tc) {
  char *str = "A - + - ";
  char *dlim = " +-";
  char *ptr;
  char *tok;
  rsize_t s1max;

  s1max = strlen(str);
  tok = strtok_s(str, &s1max, dlim, &ptr);
  assert(tok == &str[0]);
  assert(tok == ptr);
  assert(s1max == strlen(tok));
  assert(str[1] == ' ');
}

/* first, success, stop early */
ATF_TC_WITHOUT_HEAD(first_success_stop_early);
ATF_TC_BODY(first_success_stop_early, tc) {
  char *str = " A - + - ";
  char *dlim = " +-";
  rsize_t s1max;
  char *ptr;
  char *tok;

  s1max = strlen(str) - 2;
  tok = strtok_s(str, &s1max, dlim, &ptr);
  assert(tok == &str[1]);
  assert(tok == ptr);
  assert(s1max == strlen(tok) - 2);
  assert(str[0] == ' ');
  assert(str[2] == ' ');
}

/* next, success */
ATF_TC_WITHOUT_HEAD(next_success);
ATF_TC_BODY(next_success, tc) {
  char *str;
  char *dlim = " +-";
  rsize_t s1max;
  char *ptr;
  char *tok;

  str = strdup("ABC+DEF");
  ptr = str;
  s1max = strlen(str);
  tok = strtok_s(NULL, &s1max, dlim, &ptr);
  assert(tok == &str[0]);
  assert(str[0] == 'A');
  assert(str[1] == 'B');
  assert(str[2] == 'C');
  assert(str[3] == 0);
  assert(str[4] == 'D');
  assert(ptr == &str[4]);
  assert(s1max == 3);
}

/* next, success, short */
ATF_TC_WITHOUT_HEAD(next_success_short);
ATF_TC_BODY(next_success_short, tc) {
  char *str;
  char *dlim = " +-";
  rsize_t s1max;
  char *ptr;
  char *tok;

  str = strdup("ABC+DEF");
  ptr = str;
  s1max = strlen(str) - 1;
  tok = strtok_s(NULL, &s1max, dlim, &ptr);
  assert(tok == &str[0]);
  assert(str[0] == 'A');
  assert(str[1] == 'B');
  assert(str[2] == 'C');
  assert(str[3] == 0);
  assert(str[4] == 'D');
  assert(ptr == &str[4]);
  assert(s1max == 2);
}

/* next, success, short, last */
ATF_TC_WITHOUT_HEAD(next_success_short_last);
ATF_TC_BODY(next_success_short_last, tc) {
  char *str;
  char *dlim = " +-";
  rsize_t s1max;
  char *ptr;
  char *tok;

  str = strdup("ABC+DEF");
  ptr = str;
  s1max = strlen(str) - 3;
  tok = strtok_s(NULL, &s1max, dlim, &ptr);
  assert(tok == &str[0]);
  assert(str[0] == 'A');
  assert(str[1] == 'B');
  assert(str[2] == 'C');
  assert(str[3] == 0);
  assert(str[4] == 'D');
  assert(ptr == NULL);
  assert(s1max == 0);
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, s1_and_ptr_null);
  ATF_TP_ADD_TC(tp, s1_and_ptr_null_handler);
  ATF_TP_ADD_TC(tp, s1max_null);
  ATF_TP_ADD_TC(tp, s1max_null_handler);
  ATF_TP_ADD_TC(tp, s2_null);
  ATF_TP_ADD_TC(tp, s2_null_handler);
  ATF_TP_ADD_TC(tp, ptr_null);
  ATF_TP_ADD_TC(tp, ptr_null_handler);
  ATF_TP_ADD_TC(tp, s1max_gt_rsize_max);
  ATF_TP_ADD_TC(tp, s1max_gt_rsize_max_handler);
  ATF_TP_ADD_TC(tp, s1max_lt_0);
  ATF_TP_ADD_TC(tp, s1max_lt_0_handler);
  ATF_TP_ADD_TC(tp, s2_len_eq_0);
  ATF_TP_ADD_TC(tp, s2_len_eq_0_handler);
  ATF_TP_ADD_TC(tp, first_failure);
  ATF_TP_ADD_TC(tp, next_failure);
  ATF_TP_ADD_TC(tp, first_success);
  ATF_TP_ADD_TC(tp, first_success_2);
  ATF_TP_ADD_TC(tp, first_success_stop_early);
  ATF_TP_ADD_TC(tp, next_success);
  ATF_TP_ADD_TC(tp, next_success_short);
  ATF_TP_ADD_TC(tp, next_success_short_last);
  return (atf_no_error());
}
