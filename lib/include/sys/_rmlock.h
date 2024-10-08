/*
 * Copyright (c) 2010 Kip Macy All rights reserved.
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
 */

#ifndef _FSTACK_SYS__RMLOCK_H_
#define _FSTACK_SYS__RMLOCK_H_

#include "ff_host_interface.h"

struct rmlock {
    struct lock_object lock_object;
    ff_rwlock_t rm_lock;
};

struct rmlock_padalign {
    struct lock_object lock_object;
    ff_rwlock_t rm_lock;
} __aligned(CACHE_LINE_SIZE);

struct rm_queue {
	struct rm_queue	*volatile rmq_next;
	struct rm_queue	*volatile rmq_prev;
};

struct rm_priotracker {

};

#include <sys/_mutex.h>

struct rmslock_pcpu;

struct rmslock {
	struct mtx mtx;
	struct thread *owner;
	struct rmslock_pcpu *pcpu;
	int	writers;
	int	readers;
	int	debug_readers;
};

#endif    /* _FSTACK_SYS__RWLOCK_H_ */
