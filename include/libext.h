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
#ifndef _LIBEXT_H
#define _LIBEXT_H

/* Default to enabling C11 Appendix K */
#define	__EXT1_VISIBLE		1

/* 
 * User can override __EXT1_VISIBLE by doing the equivlent of
 * #define __STDC_WANT_LIB_EXT1__ 0
 */
#if defined(__STDC_WANT_LIB_EXT1__)
#undef	__EXT1_VISIBLE
#if __STDC_WANT_LIB_EXT1__
#define	__EXT1_VISIBLE		1
#else
#define	__EXT1_VISIBLE		0
#endif
#endif /* __STDC_WANT_LIB_EXT1__ */


#if __EXT1_VISIBLE
/* stddef.h */
/* ISO/IEC 9899:2011 K.3.3.2 */
#ifndef _RSIZE_T_DEFINED
#define _RSIZE_T_DEFINED
typedef size_t rsize_t;
#endif

/* stdint.h */
/* ISO/IEC 9899:2011 K.3.4.4 */
#ifndef RSIZE_MAX
#define RSIZE_MAX (SIZE_MAX >> 1)
#endif

/* stdlib.h */
#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif

/* K.3.6 */
typedef void (*constraint_handler_t)(const char * __restrict,
    void * __restrict, errno_t);
/* K.3.6.1.1 */
constraint_handler_t set_constraint_handler_s(constraint_handler_t handler);
/* K.3.6.1.2 */
_Noreturn void abort_handler_s(const char * __restrict, void * __restrict,
    errno_t);
/* K3.6.1.3 */
void ignore_handler_s(const char * __restrict, void * __restrict, errno_t);


/* string.h */
/* ISO/IEC 9899:2011 K.3.7.1.1 */
errno_t memcpy_s(void * __restrict, rsize_t, const void * __restrict, rsize_t);
/* ISO/IEC 9899:2011 K.3.7.1.2 */
errno_t memmove_s(void *, rsize_t, const void *, rsize_t);
/* ISO/IEC 9899:2011 K.3.7.1.3 */
errno_t strcpy_s(char * __restrict, rsize_t, const char * __restrict);
/* ISO/IEC 9899:2011 K.3.7.1.4 */
errno_t strncpy_s(char * __restrict, rsize_t, const char * __restrict,
    rsize_t);
/* ISO/IEC 9899:2011 K.3.7.2.1 */
errno_t strcat_s(char * __restrict, rsize_t, const char * __restrict);
/* ISO/IEC 9899:2011 K.3.7.2.2 */
errno_t strncat_s(char * __restrict, rsize_t, const char * __restrict,
    rsize_t);
/* ISO/IEC 9899:2011 K.3.7.3.1 */
char *strtok_s(char * __restrict, rsize_t * __restrict,
    const char * __restrict, char ** __restrict);
/* ISO/IEC 9899:2011 K.3.7.4.1 */
errno_t memset_s(void *, rsize_t, int, rsize_t);
/* ISO/IEC 9899:2011 K.3.7.4.2 */
errno_t strerror_s(char *, rsize_t, errno_t);
/* ISO/IEC 9899:2011 K.3.7.4.3 */
size_t strerrorlen_s(errno_t);
/* ISO/IEC 9899:2011 K.3.7.4.4 */
size_t strnlen_s(const char *, size_t);

#endif /* __EXT1_VISIBLE */
#endif /* _LIBEXT_H */
