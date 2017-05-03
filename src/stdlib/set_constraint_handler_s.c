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
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <libext.h>
#include <private_libext.h>

/*
 * Rationale recommends allocating new memory each time.
 */
static constraint_handler_t *_ch = NULL;

#if defined (__FreeBSD__)
static pthread_mutex_t ch_lock = PTHREAD_MUTEX_INITIALIZER;
#define LIBEXT_LOCK()	             \
  if (__isthreaded)                  \
    _pthread_mutex_lock(&ch_lock);
#define LIBEXT_UNLOCK()              \
  if (__isthreaded)                  \
    _pthread_mutex_unlock(&ch_lock);
#else
#define LIBEXT_LOCK()
#define LIBEXT_UNLOCK()
#endif
constraint_handler_t
set_constraint_handler_s(constraint_handler_t handler)
{
	constraint_handler_t *new, *old, ret;

	new = malloc(sizeof(constraint_handler_t));
	if (new == NULL)
		return (NULL);
	*new = handler;
	LIBEXT_LOCK();
	old = _ch;
	_ch = new;
	LIBEXT_UNLOCK();
	if (old == NULL) {
		ret = NULL;
	} else {
		ret = *old;
		free(old);
	}
	return (ret);
}

void
__throw_constraint_handler_s(const char * restrict msg, errno_t error)
{
	constraint_handler_t ch;
	LIBEXT_LOCK();
	ch = _ch != NULL ? *_ch : NULL;
	LIBEXT_UNLOCK();
	if (ch != NULL)
		ch(msg, NULL, error);
}

void
abort_handler_s(__attribute__((unused)) const char * restrict msg,
    __attribute__((unused)) void * restrict ptr, __attribute__((unused)) errno_t error)
{

	abort();
}

void
ignore_handler_s(__attribute__((unused)) const char * restrict msg,
    __attribute__((unused)) void * restrict ptr, __attribute__((unused)) errno_t error)
{
}
