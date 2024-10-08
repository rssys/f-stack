/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2007 Attilio Rao <attilio@freebsd.org>
 * Copyright (c) 2001 Jason Evans <jasone@freebsd.org>
 * Copyright (C) 2017-2021 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible 
 *    addition of one or more copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef _FSTACK_SYS_SX_H_
#define _FSTACK_SYS_SX_H_

#include_next <sys/sx.h>

#undef sx_xlocked
#define sx_xlocked(sx) (1)
#define sx_try_slock_int(sx) sx_try_slock_((sx), NULL, 0)
#define sx_try_xlock_int(sx) sx_try_xlock_((sx), NULL, 0)
#define _sx_slock_int(sx, arg) _sx_slock((sx), (arg), NULL, 0)
#define _sx_sunlock_int(sx) _sx_sunlock((sx), NULL, 0)
//#define sx_assert(sx, w) ff_rw_assert(&(sx)->sx_lock, (w), __FILE__, __LINE__)

int ff_rw_assert(ff_rwlock_t *rw, int what, const char *file, int line);

//#define sx_try_slock_int(sx) (1)
//#define sx_try_xlock_int(sx) (1)

//#define _sx_slock_int(sx, arg) (0)
//#define _sx_sunlock_int(sx) do {} while(0)

#endif    /* _FSTACK_SYS_SX_H_ */
