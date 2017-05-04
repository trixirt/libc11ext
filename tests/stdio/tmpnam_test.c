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

/* TMP_MAX >= 25 */
ATF_TC_WITHOUT_HEAD(tmp_max);
ATF_TC_BODY(tmp_max, tc) { assert(TMP_MAX >= 25); }

/* L_tmpnam bounds */
ATF_TC_WITHOUT_HEAD(L_tmpnam_bounds);
ATF_TC_BODY(L_tmpnam_bounds, tc) {
  assert(L_tmpnam > 0);
  assert(L_tmpnam < INT_MAX);
}

/* null input */
ATF_TC_WITHOUT_HEAD(null_input);
ATF_TC_BODY(null_input, tc) {
  char *s = tmpnam(NULL);
  assert(s != NULL);
  assert(strlen(s) > 0);
  assert(strlen(s) < L_tmpnam);
}

/* normal */
ATF_TC_WITHOUT_HEAD(normal);
ATF_TC_BODY(normal, tc) {
  char *r;
  char *s = (char *)malloc(L_tmpnam + 2);
  assert(s != NULL);
  memset(s, -1, L_tmpnam + 2);
  r = tmpnam(s);
  assert(r == s);
  assert(strlen(s) > 0);
  assert(strlen(s) < L_tmpnam);
  assert(s[L_tmpnam] == -1);
  assert(s[L_tmpnam + 1] == -1);
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, tmp_max);
  ATF_TP_ADD_TC(tp, L_tmpnam_bounds);
  ATF_TP_ADD_TC(tp, null_input);
  ATF_TP_ADD_TC(tp, normal);
  return (atf_no_error());
}
