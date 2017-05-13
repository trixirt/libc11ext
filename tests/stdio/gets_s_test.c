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
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <libext.h>

#include <atf-c.h>

#include "mock_stdin.h"

#define POISON -49

static errno_t e;
static const char *restrict m;

void h(const char *restrict msg, __attribute__((unused)) void *restrict ptr,
       errno_t error) {
  e = error;
  m = msg;
}

#define INIT_HANDLER()                                                         \
  e = 0;                                                                       \
  m = NULL;                                                                    \
  set_constraint_handler_s(h)

#define ERROR_HANDLER(S)                                                       \
  assert(e != 0);                                                              \
  assert(strcmp(m, S) == 0)

#define OK_HANDLER()                                                           \
  assert(e == 0);                                                              \
  assert(m == NULL)

/* nl */
ATF_TC_WITHOUT_HEAD(nl);
ATF_TC_BODY(nl, tc) {
  char i[4] = "hi\n";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, 6);
  assert(r == s);
  assert(strcmp(s, "hi") == 0);
  assert(s[3] == POISON);
  assert(s[4] == POISON);
  assert(s[5] == POISON);
  OK_HANDLER();
  memset(s, POISON, 6);
  INIT_HANDLER();
  r = gets_s(s, 6);
  assert(feof(stdin));
  assert(s[0] == '\0');
  assert(r == NULL);
  OK_HANDLER();
}

/* no nl */
ATF_TC_WITHOUT_HEAD(no_nl);
ATF_TC_BODY(no_nl, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, 6);
  assert(r == s);
  assert(strcmp(s, i) == 0);
  assert(s[4] == POISON);
  assert(s[5] == POISON);
  OK_HANDLER();
  memset(s, POISON, 6);
  INIT_HANDLER();
  r = gets_s(s, 6);
  assert(feof(stdin));
  assert(s[0] == '\0');
  assert(r == NULL);
  OK_HANDLER();
}

/* neg n */
ATF_TC_WITHOUT_HEAD(neg_n);
ATF_TC_BODY(neg_n, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, -1);
  assert(feof(stdin));
  assert(s[0] == '\0');
  assert(r == NULL);
  ERROR_HANDLER("gets_s : n > RSIZE_MAX");
}

/* zero n */
ATF_TC_WITHOUT_HEAD(zero_n);
ATF_TC_BODY(zero_n, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, 0);
  assert(feof(stdin));
  assert(s[0] == POISON);
  assert(s[1] == POISON);
  assert(r == NULL);
  ERROR_HANDLER("gets_s : n == 0");
}

/* one n */
ATF_TC_WITHOUT_HEAD(one_n);
ATF_TC_BODY(one_n, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, 1);
  assert(feof(stdin));
  assert(s[0] == '\0');
  assert(r == NULL);
  ERROR_HANDLER("gets_s : overflow buffer");
}

/* two n */
ATF_TC_WITHOUT_HEAD(two_n);
ATF_TC_BODY(two_n, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, 2);
  assert(feof(stdin));
  assert(s[0] == '\0');
  assert(r == NULL);
  ERROR_HANDLER("gets_s : overflow buffer");
}

/* s == NULL */
ATF_TC_WITHOUT_HEAD(s_null);
ATF_TC_BODY(s_null, tc) {
  char i[4] = "pie";
  char *r;
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(NULL, 2);
  assert(feof(stdin));
  assert(e != 0);
  assert(strcmp(m, "gets_s : s is NULL") == 0);
  assert(r == NULL);
}

/* nl,nl */
ATF_TC_WITHOUT_HEAD(nlnl);
ATF_TC_BODY(nlnl, tc) {
  char i[5] = "\n \n";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 4); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, 1);
  assert(r == s);
  assert(s[0] == '\0');
  assert(s[1] == POISON);
  OK_HANDLER();
}

/* drop nl */
ATF_TC_WITHOUT_HEAD(dropnl);
ATF_TC_BODY(dropnl, tc) {
  char i[5] = "  \n\n";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 4); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, 1);
  assert(r == NULL);
  assert(s[0] == '\0');
  assert(s[1] == POISON);
  ERROR_HANDLER("gets_s : overflow buffer");
  memset(s, POISON, 6);
  INIT_HANDLER();
  r = gets_s(s, 1);
  assert(r == s);
  assert(s[0] == '\0');
  assert(s[1] == POISON);
  OK_HANDLER();
}

/* eof exactly fits */
ATF_TC_WITHOUT_HEAD(eoffits);
ATF_TC_BODY(eoffits, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  INIT_HANDLER();
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = gets_s(s, 4);
  assert(feof(stdin));
  assert(strcmp(s, i) == 0);
  assert(s[5] == POISON);
  assert(r = s);
  OK_HANDLER();
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, nl);
  ATF_TP_ADD_TC(tp, no_nl);
  ATF_TP_ADD_TC(tp, neg_n);
  ATF_TP_ADD_TC(tp, zero_n);
  ATF_TP_ADD_TC(tp, one_n);
  ATF_TP_ADD_TC(tp, two_n);
  ATF_TP_ADD_TC(tp, s_null);
  ATF_TP_ADD_TC(tp, nlnl);
  ATF_TP_ADD_TC(tp, dropnl);
  ATF_TP_ADD_TC(tp, eoffits);
  return (atf_no_error());
}
