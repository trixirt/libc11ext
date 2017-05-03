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

/* s null ptr */
ATF_TC_WITHOUT_HEAD(s_null_ptr);
ATF_TC_BODY(s_null_ptr, tc) { assert(strnlen_s(0, 1) == 0); }

/* 0 maxsize */
ATF_TC_WITHOUT_HEAD(zero_maxsize);
ATF_TC_BODY(zero_maxsize, tc) {
  char s[2] = {48, 0};

  assert(strnlen_s(&s[0], 0) == 0);
}

/* short maxsize */
ATF_TC_WITHOUT_HEAD(short_maxsize);
ATF_TC_BODY(short_maxsize, tc) {
  char s[4] = {48, 48, 48, 0};

  assert(strnlen_s(&s[0], 2) == 2);
}

/* long maxsize */
ATF_TC_WITHOUT_HEAD(long_maxsize);
ATF_TC_BODY(long_maxsize, tc) {
  char s[6] = {48, 48, 48, 0, 48, 48};

  assert(strnlen_s(&s[0], 6) == 3);
}

/* correct maxsize */
ATF_TC_WITHOUT_HEAD(correct_maxsize);
ATF_TC_BODY(correct_maxsize, tc) {
  char s[6] = {48, 48, 48, 0, 48, 48};

  assert(strnlen_s(&s[0], 3) == 3);
}

/* correct maxsize with null */
ATF_TC_WITHOUT_HEAD(correct_maxsize_with_null);
ATF_TC_BODY(correct_maxsize_with_null, tc) {
  char s[6] = {48, 48, 48, 0, 48, 48};

  assert(strnlen_s(&s[0], 4) == 3);
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, s_null_ptr);
  ATF_TP_ADD_TC(tp, zero_maxsize);
  ATF_TP_ADD_TC(tp, short_maxsize);
  ATF_TP_ADD_TC(tp, long_maxsize);
  ATF_TP_ADD_TC(tp, correct_maxsize);
  ATF_TP_ADD_TC(tp, correct_maxsize_with_null);
  return (atf_no_error());
}
