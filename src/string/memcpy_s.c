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

/* ISO/IEC 9899:2011 K.3.7.1.1 */
errno_t memcpy_s(void *restrict s1, rsize_t s1max, const void *restrict s2,
                 rsize_t n) {
  rsize_t lim;
  uintptr_t s1e, s2e;
  errno_t ret;
  int zero_fill;

  ret = EINVAL;
  zero_fill = 0;

  if (s1 == NULL) {
    __throw_constraint_handler_s("memcpy_s : s1 is NULL", ret);
  } else if (s1max > RSIZE_MAX) {
    __throw_constraint_handler_s("memcpy_s : s1max > RSIZE_MAX", ret);
  } else if (s2 == NULL) {
    zero_fill = 1;
    __throw_constraint_handler_s("memcpy_s : s2 is NULL", ret);
  } else if (n > RSIZE_MAX) {
    zero_fill = 1;
    __throw_constraint_handler_s("memcpy_s : n > RSIZE_MAX", ret);
  } else {
    lim = s1max;
    if (n < s1max)
      lim = n;
    if (lim > 0) {
      s1e = (uintptr_t)s1 + lim;
      s2e = (uintptr_t)s2 + lim;
      if (((uintptr_t)s1 >= (uintptr_t)s2 && (uintptr_t)s1 < s2e) ||
          ((uintptr_t)s2 >= (uintptr_t)s1 && (uintptr_t)s2 < s1e)) {
        /* do not zero fill on overlap */
        __throw_constraint_handler_s("memcpy_s : memory overlaps", ret);
      } else {
        memcpy(s1, s2, lim);
        ret = 0;
      }
    } else {
      /* trivial, do nothing */
      ret = 0;
    }
  }
  if (ret && zero_fill)
    memset_s(s1, s1max, 0, s1max);
  return (ret);
}
