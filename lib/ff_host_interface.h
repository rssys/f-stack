/*
 * Copyright (c) 2013 Patrick Kelsey. All rights reserved.
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
 * Derived in part from libuinet's uinet_host_interface.h.
 */

#ifndef _FSTACK_HOST_INTERFACE_H_
#define _FSTACK_HOST_INTERFACE_H_

#define ff_PROT_NONE     0x00
#define ff_PROT_READ     0x01
#define ff_PROT_WRITE    0x02

#define ff_MAP_SHARED    0x0001
#define ff_MAP_PRIVATE   0x0002
#define ff_MAP_ANON      0x1000
#define ff_MAP_NOCORE    0x00020000

#define ff_MAP_FAILED    ((void *)-1)

#define ff_MTX_SPIN      0x00000001
#define ff_MTX_RECURSE   0x00000004

#define ff_RW_RECURSE    0x1

void *ff_mmap(void *addr, uint64_t len, int prot, int flags, int fd, uint64_t offset);
int ff_munmap(void *addr, uint64_t len);

void *ff_malloc(uint64_t size);
void *ff_calloc(uint64_t number, uint64_t size);
void *ff_realloc(void *p, uint64_t size);
void ff_free(void *p);

#define ff_CLOCK_REALTIME        0
#define ff_CLOCK_MONOTONIC        4
#define ff_CLOCK_MONOTONIC_FAST       12

#define ff_NSEC_PER_SEC    (1000ULL * 1000ULL * 1000ULL)

void ff_clock_gettime(int id, int64_t *sec, long *nsec);
uint64_t ff_clock_gettime_ns(int id);
uint64_t ff_get_tsc_ns(void);

void ff_get_current_time(int64_t *sec, long *nsec);
void ff_update_current_ts(void);

typedef void * ff_cond_t;
//typedef struct {
//    char lock[64];
//} ff_rwlock_t;

typedef struct {
    volatile int locked;
    int recursive;
    int count;
    void *owner;
} ff_mutex_t;

typedef ff_mutex_t ff_rwlock_t;

void ff_arc4rand(void *ptr, unsigned int len, int reseed);
uint32_t ff_arc4random(void);

int ff_setenv(const char *name, const char *value);
char *ff_getenv(const char *name);

void ff_os_errno(int error);

int ff_in_pcbladdr(uint16_t family, void *faddr, uint16_t fport, void *laddr);

int ff_rss_check(void *softc, uint32_t saddr, uint32_t daddr,
    uint16_t sport, uint16_t dport);

static inline void ff_mtx_init_lock(ff_mutex_t *m, int flags)
{
    m->locked = 0;
    m->recursive = (flags & ff_MTX_RECURSE) != 0;
    m->count = 0;
    m->owner = NULL;
}

static inline void ff_mtx_destroy_lock(ff_mutex_t *m)
{}

static inline void *gettp(void)
{
    void *tp;
    __asm__ __volatile__ ("movq %%fs:0, %0" : "=r" (tp));
    return tp;
}

static inline void ff_mtx_lock_(ff_mutex_t *m)
{
    while (1) {
        if (!__atomic_exchange_n(&m->locked, 1, __ATOMIC_ACQUIRE)) {
            return;
        }
        while (__atomic_load_n(&m->locked, __ATOMIC_RELAXED)) {
            __builtin_ia32_pause();
        }
    }
}

static inline void ff_mtx_lock(ff_mutex_t *m)
{
    if (m->recursive) {
        void *tp = gettp();
        if (m->owner != tp) {
            ff_mtx_lock_(m);
            m->owner = tp;
        }
        ++m->count;
    } else {
        ff_mtx_lock_(m);
    }
}

static inline void ff_mtx_lock_spin(ff_mutex_t *m)
{
    ff_mtx_lock(m);
}

static inline void ff_mtx_unlock_(ff_mutex_t *m)
{
    __atomic_store_n(&m->locked, 0, __ATOMIC_RELEASE);
}

static inline void ff_mtx_unlock(ff_mutex_t *m)
{
    if (m->recursive) {
        if (--m->count == 0) {
            m->owner = NULL;
            ff_mtx_unlock_(m);
        }
    } else {
        ff_mtx_unlock_(m);
    }
}

static inline void ff_mtx_unlock_spin(ff_mutex_t *m)
{
    ff_mtx_unlock(m);
}

static inline int ff_mtx_trylock_(ff_mutex_t *m)
{
    return !__atomic_load_n(&m->locked, __ATOMIC_RELAXED) &&
        !__atomic_exchange_n(&m->locked, 1, __ATOMIC_ACQUIRE);
}

static inline int ff_mtx_trylock(ff_mutex_t *m)
{
    if (m->recursive) {
        void *tp = gettp();
        if (m->owner != tp) {
            if (ff_mtx_trylock_(m) == 0) {
                return 0;
            }
            m->owner = tp;
            ++m->count;
            return 1;
        }
        return 1;
    } else {
        return ff_mtx_trylock_(m);
    }
}

static inline int ff_mtx_trylock_spin(ff_mutex_t *m)
{
    return ff_mtx_trylock(m);
}

static inline void *ff_mtx_owner(ff_mutex_t *m)
{
    return m->owner;
}

static inline int ff_mtx_assert(ff_mutex_t *m, int what, const char *file, int line)
{
    return 1;
}

#define FF_RW_WRITER    1
#define FF_RW_READER    2

/*
static inline void ff_rw_init_lock(ff_rwlock_t *rw, int flags)
{
    rw->bits = 0;
    rw->recursive = (flags & ff_RW_RECURSE) != 0;
    rw->count = 0;
    rw->owner = NULL;
}

static inline void ff_rw_destroy_lock(ff_rwlock_t *rw)
{}

static inline int ff_rw_try_wlock_(ff_rwlock_t *rw)
{
    int expected = 0;
    return __atomic_compare_exchange_n(&rw->bits, &expected, FF_RW_WRITER, false,
        __ATOMIC_ACQ_REL, __ATOMIC_RELAXED);
}

static inline int ff_rw_try_wlock(ff_rwlock_t *rw)
{
    if (rw->recursive) {
        void *tp = gettp();
        if (rw->owner != tp) {
            if (ff_rw_try_wlock_(rw) == 0) {
                return 0;
            }
            rw->owner = tp;
            ++rw->count;
            return 1;
        }
        return 1;
    } else {
        return ff_rw_try_wlock_(rw);
    }
}

static inline void ff_rw_wlock_(ff_rwlock_t *rw)
{
    while (!ff_rw_try_wlock_(rw));
}

static inline void ff_rw_wlock(ff_rwlock_t *rw)
{
    if (rw->recursive) {
        void *tp = gettp();
        if (rw->owner != tp) {
            ff_rw_wlock_(rw);
            rw->owner = tp;
        }
        ++rw->count;
    } else {
        ff_rw_wlock_(rw);
    }
}

static inline void ff_rw_wunlock(ff_rwlock_t *rw)
{
    if (rw->recursive) {
        if (--rw->count == 0) {
            rw->owner = NULL;
            __atomic_fetch_and(&rw->bits, ~FF_RW_WRITER, __ATOMIC_RELEASE);
        }
    } else {
        __atomic_fetch_and(&rw->bits, ~FF_RW_WRITER, __ATOMIC_RELEASE);
    }
}

static inline void ff_rw_runlock(ff_rwlock_t *rw)
{
    __atomic_fetch_add(&rw->bits, -FF_RW_READER, __ATOMIC_RELEASE);
}

static inline int ff_rw_try_rlock(ff_rwlock_t *rw)
{
    int value = __atomic_fetch_add(&rw->bits, FF_RW_READER, __ATOMIC_ACQUIRE);
    if (value & FF_RW_WRITER) {
        __atomic_fetch_add(&rw->bits, -FF_RW_READER, __ATOMIC_RELEASE);
        return 0;
    }
    return 1;
}

static inline void ff_rw_rlock(ff_rwlock_t *rw)
{
    while (!ff_rw_try_rlock(rw));
}

static inline int ff_rw_try_upgrade(ff_rwlock_t *rw)
{
    int expected = FF_RW_READER;
    return __atomic_compare_exchange_n(&rw->bits, &expected, FF_RW_WRITER, false,
        __ATOMIC_ACQ_REL, __ATOMIC_RELAXED);
}

static inline void ff_rw_downgrade(ff_rwlock_t *rw)
{
    __atomic_fetch_add(&rw->bits, FF_RW_READER, __ATOMIC_ACQUIRE);
    ff_rw_wunlock(rw);
}

static inline int ff_rw_wowned(ff_rwlock_t *rw)
{
    return __atomic_load_n(&rw->bits, __ATOMIC_ACQUIRE) & FF_RW_WRITER;
}

static inline int ff_rw_assert(ff_rwlock_t *rw, int what, const char *file, int line)
{
    return 1;
}
*/

static inline void ff_rw_init_lock(ff_rwlock_t *rw, int flags)
{
    rw->locked = 0;
    rw->recursive = (flags & ff_RW_RECURSE) != 0;
    rw->count = 0;
    rw->owner = NULL;
}

static inline void ff_rw_destroy_lock(ff_rwlock_t *rw)
{}

static inline int ff_rw_try_wlock(ff_rwlock_t *rw)
{
    return ff_mtx_trylock(rw);
}

static inline void ff_rw_wlock(ff_rwlock_t *rw)
{
    ff_mtx_lock(rw);
}

static inline void ff_rw_wunlock(ff_rwlock_t *rw)
{
    ff_mtx_unlock(rw);
}

static inline void ff_rw_runlock(ff_rwlock_t *rw)
{
    ff_mtx_unlock(rw);
}

static inline int ff_rw_try_rlock(ff_rwlock_t *rw)
{
    return ff_mtx_trylock(rw);
}

static inline void ff_rw_rlock(ff_rwlock_t *rw)
{
    ff_mtx_lock(rw);
}

static inline int ff_rw_try_upgrade(ff_rwlock_t *rw)
{
    return 1;
}

static inline void ff_rw_downgrade(ff_rwlock_t *rw)
{
}

static inline int ff_rw_wowned(ff_rwlock_t *rw)
{
    return 1;
}

static inline int ff_rw_assert(ff_rwlock_t *rw, int what, const char *file, int line)
{
    return 1;
}

void ff_uma_page_slab_hash_lock(void);
void ff_uma_page_slab_hash_unlock(void);
void ff_host_init_thread(void);

#endif

