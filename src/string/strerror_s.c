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
#include <string.h>
#include <libext.h>
#include <private_libext.h>

/* ISO/IEC 9899:2011 K.3.7.4.2 */
errno_t strerror_s(char *s, rsize_t maxsize, errno_t errnum) {
  errno_t ret;
  size_t err_len;
  char *err_str;
  char *e = "...";

  ret = EINVAL;
  if (s == NULL) {
    __throw_constraint_handler_s("strerror_s : s is NULL", ret);
  } else if (maxsize > RSIZE_MAX) {
    __throw_constraint_handler_s("strerror_s : maxsize > RSIZE_MAX", ret);
  } else if (maxsize == 0) {
    __throw_constraint_handler_s("strerror_s : maxsize == 0", ret);
  } else {
    err_len = strerrorlen_s(errnum) + 1;
    err_str = strerror(errnum);
    if (err_len <= maxsize) {
      strcpy_s(s, err_len, err_str);
      ret = 0;
    } else {
      if (maxsize < 5) {
        strcpy_s(s, maxsize, &e[4 - maxsize]);
      } else {
        /*
         * err_str is going to be truncated
         * use normal call
         */
        strncpy(s, err_str, maxsize - 4);
        /*
         * strncpy does not null terminate
         * so advance to end
         */
        strcpy_s(&s[maxsize - 4], 4, e);
      }
    }
  }
  return (ret);
}
