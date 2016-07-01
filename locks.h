#ifndef LOCKS_H
#define LOCKS_H


#ifdef WIN32
#include <windows.h>
#include <process.h>

typedef LONG lock_basic_t;
void lock_basic_init(lock_basic_t *lock);
void lock_basic_destroy(lock_basic_t *lock);
void lock_basic_lock(lock_basic_t *lock);
void lock_basic_unlock(lock_basic_t *lock);

typedef HANDLE sdns_thread_t;
void sdns_thread_create(sdns_thread_t *thr, void *(*func)(void*), void *arg);
void sdns_thread_detach(sdns_thread_t thr);
sdns_thread_t sdns_thread_self(void);
void sdns_thread_join(sdns_thread_t thr);

#else
#include <pthread.h>

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
