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
#include <stdlib.h>

#include <atf-c.h>

/* trash name */
ATF_TC_WITHOUT_HEAD(trash_name);
ATF_TC_BODY(trash_name, tc) {
  const unsigned char utn[4] = {255, 254, 253, 0};
  const char *tn = (char *)&utn[0];
  assert(getenv(tn) == NULL);
}

/* close name */
ATF_TC_WITHOUT_HEAD(close_name);
ATF_TC_BODY(close_name, tc) {
  char *rn = "getenv_test_env=habanero";
  if (!putenv(rn)) {
    /* long 1 */
    assert(getenv("getenv_test_enva") == NULL);
    /* short 1 */
    assert(getenv("getenv_test_en") == NULL);
  }
}

/* normal */
ATF_TC_WITHOUT_HEAD(normal);
ATF_TC_BODY(normal, tc) {
  char *rn = "getenv_test_env=habanero";
  if (!putenv(rn)) {
    char *t = getenv("getenv_test_env");
    assert(t != NULL);
    assert(strlen(t) == strlen("habanero"));
    assert(strcmp(t, "habanero") == 0);
  }
}

/* empty */
ATF_TC_WITHOUT_HEAD(empty);
ATF_TC_BODY(empty, tc) {
  char *rn = "getenv_test_env_empty=";
  if (!putenv(rn)) {
    char *t = getenv("getenv_test_env_empty");
    assert(t != NULL);
    assert(strlen(t) == 0);
  }
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, trash_name);
  ATF_TP_ADD_TC(tp, close_name);
  ATF_TP_ADD_TC(tp, normal);
  ATF_TP_ADD_TC(tp, empty);
  return (atf_no_error());
}
