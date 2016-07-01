#include "locks.h"

#ifdef WIN32

void lock_basic_init(lock_basic_t *lock)
{
    (void)InterlockedExchange(lock, 0);
}

void lock_basic_destroy(lock_basic_t *lock)
{
    (void)InterlockedExchange(lock, 0);
}

void lock_basic_lock(lock_basic_t *lock)
{
    LONG wait = 1;

    while (InterlockedExchange(lock, 1)) {
        Sleep(wait);
        wait *= 2;
    }
}

void lock_basic_unlock(lock_basic_t *lock)
{
    (void)InterlockedExchange(lock, 0);
}

void sdns_thread_create(sdns_thread_t *thr, void *(*func)(void*), void *arg)
{
    *thr = (sdns_thread_t)_beginthreadex(NULL, 0, (void*)func, arg, 0, NULL);
    if (*thr == NULL) {
        fprintf(stderr, "CreateThread failed");
        exit(1);
    }
}

void sdns_thread_detach(sdns_thread_t thr)
{
    CloseHandle(thr);
}

sdns_thread_t sdns_thread_self(void)
{
    return GetCurrentThread();
}

void sdns_thread_join(sdns_thread_t thr)
{
    WaitForSingleObject(thr, INFINITE);
    CloseHandle(thr);
}

#endif

#ifndef __APPLE__

/** global lock list for openssl locks */
static lock_basic_t *sdns_openssl_locks = NULL;

static void sdns_openssl_lock_cb(int mode, int type, const char *file, int line)
{
    if((mode&CRYPTO_LOCK)) {
        lock_basic_lock(&sdns_openssl_locks[type]);
    } else {
        lock_basic_unlock(&sdns_openssl_locks[type]);
    }
}

/** callback that gets thread id for openssl */
#if OPENSSL_VERSION_NUMBER >= OPENSSL_VERSION_1_0_0
    static int openssl_set_id_callback(void (*threadid_func)(CRYPTO_THREADID *))
    {
        return CRYPTO_THREADID_set_callback(threadid_func);
    }
    static void sdns_openssl_id_cb(CRYPTO_THREADID * id)
    {
        CRYPTO_THREADID_set_numeric(id, (unsigned long)sdns_thread_self());
    }
#else
    static void openssl_set_id_callback(unsigned long (func)(void))
    {
        CRYPTO_set_id_callback(func);
    }
    static unsigned long sdns_openssl_id_cb(void)
    {
        return (unsigned long)sdns_thread_self();
    }
#endif

int sdns_openssl_lock_init(void)
{
    int i;
    sdns_openssl_locks = (lock_basic_t*)malloc(
        sizeof(lock_basic_t)*CRYPTO_num_locks());
    if(!sdns_openssl_locks)
        return 0;
    for(i=0; i<CRYPTO_num_locks(); i++) {
        lock_basic_init(&sdns_openssl_locks[i]);
    }

    //openssl 1.0.0 CRYPTO_set_id_callback was replaced by CRYPTO_THREADID_set_callback
    openssl_set_id_callback(sdns_openssl_id_cb);

    CRYPTO_set_locking_callback(&sdns_openssl_lock_cb);
    return 1;
}

void sdns_openssl_lock_delete(void)
{
    int i;
    if(!sdns_openssl_locks)
        return;
    openssl_set_id_callback(NULL);
    CRYPTO_set_locking_callback(NULL);
    for(i=0; i<CRYPTO_num_locks(); i++) {
        lock_basic_destroy(&sdns_openssl_locks[i]);
    }
    free(sdns_openssl_locks);
}

#endif /* __APPLE__ */
