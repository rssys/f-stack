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

#ifndef _FSTACK_SYS_MUTEX_H_
#define _FSTACK_SYS_MUTEX_H_
#include_next <sys/mutex.h>

#undef mtx_lock
#undef mtx_lock_spin
#undef mtx_trylock
#undef mtx_trylock_spin
#undef mtx_unlock
#undef mtx_unlock_spin

#undef thread_unlock

#undef mtx_init
#undef mtx_destroy
#undef mtx_owned
#undef mtx_owner
//#undef mtx_assert

#define DO_NOTHING do {} while(0)
#define CAUSE_ERROR _Static_assert(false, "")

#define mtx_lock(m) ff_mtx_lock(&(m)->mtx_lock)
#define mtx_lock_spin(m) ff_mtx_lock_spin(&(m)->mtx_lock)
#define mtx_trylock(m)		ff_mtx_trylock(&(m)->mtx_lock)
#define mtx_trylock_spin(m)	ff_mtx_trylock_spin(&(m)->mtx_lock)
#define mtx_unlock(m)		ff_mtx_unlock(&(m)->mtx_lock)
#define mtx_unlock_spin(m)	ff_mtx_unlock_spin(&(m)->mtx_lock)
//#define mtx_assert(m, w)    ff_mtx_assert(&(m)->mtx_lock, (w), __FILE__, __LINE__)

#define	thread_unlock(tdp) mtx_unlock((tdp)->td_lock)

void ff_mtx_init(struct lock_object *lo, const char *name, const char *type, int opts);
void ff_mtx_destroy(struct lock_object *lo);
void ff_mtx_lock(ff_mutex_t *m);
void ff_mtx_lock_spin(ff_mutex_t *m);
void ff_mtx_unlock(ff_mutex_t *m);
void ff_mtx_unlock_spin(ff_mutex_t *m);
int ff_mtx_trylock(ff_mutex_t *m);
int ff_mtx_trylock_spin(ff_mutex_t *m);
void *ff_mtx_owner(ff_mutex_t *m);
int ff_mtx_assert(ff_mutex_t *m, int what, const char *file, int line);

#define mtx_init(m, n, t, o) \
    ff_mtx_init(&(m)->lock_object, n, t, o)

#define mtx_destroy(m) \
    ff_mtx_destroy(&(m)->lock_object)

#define mtx_owned(m)    (mtx_owner(m) != NULL)
#define mtx_owner(m)    ((struct thread *)ff_mtx_owner(&(m)->mtx_lock))

#endif    /* _FSTACK_SYS_MUTEX_H_ */
