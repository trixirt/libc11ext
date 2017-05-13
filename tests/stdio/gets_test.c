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

#define POISON -31

/*
 * gets should have been removed from stdio.h
 * declare here
 */
char *gets(char *s);

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
  r = gets(s);
  assert(r == s);
  assert(strcmp(s, "hi") == 0);
  assert(s[3] == POISON);
  assert(s[4] == POISON);
  assert(s[5] == POISON);
  memset(s, POISON, 6);
  r = gets(s);
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
  r = gets(s);
  assert(r == s);
  assert(strcmp(s, i) == 0);
  assert(s[4] == POISON);
  assert(s[5] == POISON);
  memset(s, POISON, 6);
  r = gets(s);
  assert(feof(stdin));
  assert(s[0] == POISON);
  assert(r == NULL);
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, nl);
  ATF_TP_ADD_TC(tp, no_nl);
  return (atf_no_error());
}
