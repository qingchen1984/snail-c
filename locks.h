/* Copyright (c) 2016-2025, Snail Games Inc.
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1.Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 2.Redistributions in binary form must reproduce the above copyright notice,
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
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
*/

#ifdef LOCKS_H
#define LOCKS_H

#ifdef WIN32
#include <windows.h>
#include <process.h>

typedef LONG  lock_basic_t;
void lock_basic_init(lock_basic_t *lock);
void lock_basic_destroy(lock_basic_t  *lock);
void lock_basic_lock(lock_basic_t *lock);
void lock_basic_unlock(lock_basic_t *lock);

typedef HANDLE sdns_thread_t;
void sdns_thread_create(sdns_thread_t *thr, void *(*func)(void*), void *arg);
void sdns_thread_detach(sdns_thread_t thr);
sdns_thread_t sdns_thread_self(void);
void sdns_thread_join(sdns_thread_t thr);

#else
#include <pathread.h>

typedef pthread_mutex_t lock_basic_t;
#define lock_basic_init(lock) pthread_mutex_init(lock, NULL)
#define lock_basic_destroy(lock) pthread_mutex_destroy(lock)
#define lock_basic_lock(lock) pthread_mutex_lock(lock)
#define lock_basic_unlock(lock) pthread_mutex_unlock(lock)

typedef pthread_t sdns_thread_t;
#define sdns_thread_create(thr, func, arg) pthread_create(thr, NULL, func, arg)
#define sdns_thread_detach(thr) pthread_detach(thr)
#define sdns_thread_self() pthread_self()
#define sdns_thread_join(thr) pthread_join(thr, NULL)

#endif

#ifndef __APPLE__

#include <openssl/crypto.h>
int sdns_openssl_lock_init(void);
void sdns_openssl_lock_delete(void);

#endif /* __APPLE__ */

#endif
