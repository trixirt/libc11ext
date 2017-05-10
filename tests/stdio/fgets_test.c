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

#include <atf-c.h>

#include "mock_stdin.h"

#define POISON -67

/* nl */
ATF_TC_WITHOUT_HEAD(nl);
ATF_TC_BODY(nl, tc) {
  char i[4] = "hi\n";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = fgets(s, 6, stdin);
  assert(r == s);
  assert(strcmp(s, i) == 0);
  assert(s[4] == POISON);
  assert(s[5] == POISON);
  memset(s, POISON, 6);
  r = fgets(s, 6, stdin);
  assert(feof(stdin));
  assert(s[0] == POISON);
  assert(r == NULL);
}

/* no nl */
ATF_TC_WITHOUT_HEAD(no_nl);
ATF_TC_BODY(no_nl, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = fgets(s, 6, stdin);
  assert(r == s);
  assert(strcmp(s, i) == 0);
  assert(s[4] == POISON);
  assert(s[5] == POISON);
  memset(s, POISON, 6);
  r = fgets(s, 6, stdin);
  assert(feof(stdin));
  assert(s[0] == POISON);
  assert(r == NULL);
}

/* neg n */
ATF_TC_WITHOUT_HEAD(neg_n);
ATF_TC_BODY(neg_n, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = fgets(s, -1, stdin);
  assert(0 == feof(stdin));
  assert(s[0] == POISON);
  assert(r == NULL);
}

/* zero n */
ATF_TC_WITHOUT_HEAD(zero_n);
ATF_TC_BODY(zero_n, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = fgets(s, 0, stdin);
  assert(0 == feof(stdin));
  assert(s[0] == POISON);
  assert(r == NULL);
}

/* one n */
ATF_TC_WITHOUT_HEAD(one_n);
ATF_TC_BODY(one_n, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = fgets(s, 1, stdin);
  assert(0 == feof(stdin));
  assert(s[0] == '\0');
  assert(s[1] == POISON);
  assert(r == s);
}

/* two n */
ATF_TC_WITHOUT_HEAD(two_n);
ATF_TC_BODY(two_n, tc) {
  char i[4] = "pie";
  char s[6];
  char *r;
  memset(s, POISON, 6);
  MOCK_STDIN_SETUP();
  MOCK_STDIN_WRITE(&i[0], 3); /* do not write the null */
  MOCK_STDIN_CLOSE();
  r = fgets(s, 2, stdin);
  assert(0 == feof(stdin));
  assert(s[0] == 'p');
  assert(s[1] == '\0');
  assert(s[2] == POISON);
  assert(r == s);
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, nl);
  ATF_TP_ADD_TC(tp, no_nl);
  ATF_TP_ADD_TC(tp, neg_n);
  ATF_TP_ADD_TC(tp, zero_n);
  ATF_TP_ADD_TC(tp, one_n);
  ATF_TP_ADD_TC(tp, two_n);
  return (atf_no_error());
}
