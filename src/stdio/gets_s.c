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

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <libext.h>
#include <private_libext.h>

/* K.3.5.4.1 */
char *gets_s(char *s, rsize_t n) {
  char *ret = NULL;
  int zero_fill = 0;
  int io_error = 0;
  int eof_state = 0;

  if (s == NULL) {
    __throw_constraint_handler_s("gets_s : s is NULL", EINVAL);
  } else if (n > RSIZE_MAX) {
    __throw_constraint_handler_s("gets_s : n > RSIZE_MAX", EINVAL);
    zero_fill = 1;
  } else if (n == 0) {
    __throw_constraint_handler_s("gets_s : n == 0", EINVAL);
  } else {
    rsize_t i;
    rsize_t lim = n;
    for (i = 0; i < lim; i++) {
      int c;
      c = fgetc(stdin);
      if (c == EOF) {
        if (feof(stdin)) {
          eof_state = 1;
        } else {
          /* LCOV_EXCL_START i/o error not handled in tests */
          io_error = 1;
          zero_fill = 1;
          /* LCOV_EXCL_STOP */
        }
        break;
      } else if (c == '\n') {
        break;
      } else {
        s[i] = (char)c;
      }
    }
    if (io_error) {
      /* LCOV_EXCL_START i/o error not handled in tests */
      zero_fill = 1;
      __throw_constraint_handler_s("gets_s : i/o error", EINVAL);
      /* LCOV_EXCL_STOP */
    } else if (i == lim) {
      zero_fill = 1;
      __throw_constraint_handler_s("gets_s : overflow buffer", EINVAL);
    } else {
      s[i] = '\0';
      if (!eof_state || i != 0)
        ret = s;
    }
  }
  if (ret == NULL) {
    if (zero_fill)
      s[0] = '\0';
    if (!io_error && !eof_state) {
      while (1) {
        int c;
        c = fgetc(stdin);
        if (c == EOF)
          break;
        else if (c == '\n')
          break;
      }
    }
  }
  return (ret);
}
