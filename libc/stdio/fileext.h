/*	$OpenBSD: fileext.h,v 1.2 2005/06/17 20:40:32 espie Exp $	*/
/* $NetBSD: fileext.h,v 1.5 2003/07/18 21:46:41 nathanw Exp $ */

/*-
 * Copyright (c)2001 Citrus Project,
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Citrus$
 */

#include <pthread.h>
#include "wcio.h"

/*
 * file extension
 */
struct __sfileext {
	struct	__sbuf _ub; /* ungetc buffer */
	struct wchar_io_data _wcio;	/* wide char io status */
	pthread_mutex_t _lock; /* file lock */
};

#define _FILEEXT_INITIALIZER  {{NULL,0},{0},PTHREAD_RECURSIVE_MUTEX_INITIALIZER}

#define _EXT(fp) ((struct __sfileext *)((fp)->_ext._base))
#define _UB(fp) _EXT(fp)->_ub
#define _FLOCK(fp)  _EXT(fp)->_lock

#define _FILEEXT_INIT(fp) \
do { \
	_UB(fp)._base = NULL; \
	_UB(fp)._size = 0; \
	WCIO_INIT(fp); \
	_FLOCK_INIT(fp); \
} while (0)

/* Helper macros to avoid a function call when you know that fp is not NULL.
 * Notice that we keep _FLOCK_INIT() fast by slightly breaking our pthread
 * encapsulation.
 */
#define _FLOCK_INIT(fp)    _FLOCK(fp).value = __PTHREAD_RECURSIVE_MUTEX_INIT_VALUE
#define _FLOCK_LOCK(fp)    pthread_mutex_lock(&_FLOCK(fp))
#define _FLOCK_TRYLOCK(fp) pthread_mutex_trylock(&_FLOCK(fp))
#define _FLOCK_UNLOCK(fp)  pthread_mutex_unlock(&_FLOCK(fp))

#define _FILEEXT_SETUP(f, fext) \
do { \
	(f)->_ext._base = (unsigned char *)(fext); \
	_FILEEXT_INIT(f); \
} while (0)
