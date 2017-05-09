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

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <atf-c.h>

/*
 * gets should have been removed from stdio.h
 * declare here
 */
char *gets(char *s);

/* pipe and replace in pipe with stdin */
ATF_TC_WITHOUT_HEAD(pipe_setup);
ATF_TC_BODY(pipe_setup, tc) {
  int p[2] = {-1, -1};
  int d;
  assert(p[0] == -1);
  assert(p[1] == -1);
  assert(pipe(p) == 0);
  assert(p[0] > 0);
  assert(p[1] > 0);
  assert(p[0] != STDIN_FILENO);
  d = dup2(p[0], STDIN_FILENO);
  assert(d == STDIN_FILENO);
}

ATF_TC_WITHOUT_HEAD(pipe_gets);
ATF_TC_BODY(pipe_gets, tc) {
  int p[2];
  char m[16] = "hello\ngoodbye";
  char r[16] = "";
  size_t c = strlen(m) + 1;
  char *s;
  pipe(p);
  if (c == write(p[1], &m[0], c)) {
    close(p[1]);
  }
  dup2(p[0], STDIN_FILENO);

  s = gets(&r[0]);
  assert(s == &r[0]);
  assert(strcmp(s, "hello") == 0);
  r[0] = 0;
  s = gets(&r[0]);
  assert(s == &r[0]);
  assert(feof(stdin) != 0);
  assert(strcmp(s, "goodbye") == 0);
  s = gets(&r[0]);
  assert(s == NULL);
}

ATF_TP_ADD_TCS(tp) {
  ATF_TP_ADD_TC(tp, pipe_setup);
  ATF_TP_ADD_TC(tp, pipe_gets);
  return (atf_no_error());
}
