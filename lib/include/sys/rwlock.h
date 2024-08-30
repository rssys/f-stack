/*
 * Copyright (C) 2017-2021 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _FSTACK_SYS_RWLOCK_H_
#define _FSTACK_SYS_RWLOCK_H_
#include_next <sys/rwlock.h>

#undef rw_init
#undef rw_init_flags
#undef rw_destroy
#undef rw_wowned
#undef _rw_wlock
#undef _rw_try_wlock
#undef _rw_wunlock
#undef _rw_rlock
#undef _rw_try_rlock
#undef _rw_runlock
#undef _rw_try_upgrade
#undef _rw_downgrade
#undef rw_wlock
#undef rw_try_wlock
#undef rw_wunlock
#undef rw_rlock
#undef rw_try_rlock
#undef rw_runlock
#undef rw_try_upgrade
#undef rw_downgrade
//#undef rw_assert

#define DO_NOTHING do {} while(0)

void ff_rw_init_flags(struct lock_object *lo, const char *name, int opts);
void ff_rw_destroy(struct lock_object *lo);
void ff_rw_wlock(ff_rwlock_t *rw);
int ff_rw_try_wlock(ff_rwlock_t *rw);
void ff_rw_rlock(ff_rwlock_t *rw);
int ff_rw_try_rlock(ff_rwlock_t *rw);
int ff_rw_try_upgrade(ff_rwlock_t *rw);
void ff_rw_downgrade(ff_rwlock_t *rw);
int ff_rw_wowned(ff_rwlock_t *rw);
int ff_rw_assert(ff_rwlock_t *rw, int what, const char *file, int line);

#define rw_init(rw, n)          \
    rw_init_flags((rw), (n), 0)
#define rw_init_flags(rw, n, o) \
    ff_rw_init_flags(&(rw)->lock_object, (n), (o))
#define rw_destroy(rw) ff_rw_destroy(&(rw)->lock_object)
#define rw_wlock(rw) ff_rw_wlock(&(rw)->rw_lock)
#define rw_try_wlock(rw) ff_rw_try_wlock(&(rw)->rw_lock)
#define rw_wunlock(rw) ff_rw_wunlock(&(rw)->rw_lock)
#define rw_rlock(rw) ff_rw_rlock(&(rw)->rw_lock)
#define rw_try_rlock(rw) ff_rw_try_rlock(&(rw)->rw_lock)
#define rw_runlock(rw) ff_rw_runlock(&(rw)->rw_lock)
#define rw_try_upgrade(rw) ff_rw_try_upgrade(&(rw)->rw_lock)
#define rw_downgrade(rw) ff_rw_downgrade(&(rw)->rw_lock)
#define rw_wowned(rw) ff_rw_wowned(&(rw)->rw_lock)
//#define rw_assert(m, w) ff_rw_assert(&(m)->rw_lock, (w), __FILE__, __LINE__)

#endif    /* _FSTACK_SYS_RWLOCK_H_ */
