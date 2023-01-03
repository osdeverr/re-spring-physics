#pragma once

// Thank you, Magorx

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#elif __linux__
#include <cstdint>
#include <errno.h>
#include <semaphore.h>
#else
#include <semaphore>
#endif

#ifdef __APPLE__

class Semaphore {
    dispatch_semaphore_t sem;

public:
    inline Semaphore(uint32_t value = 0) {
        sem = dispatch_semaphore_create(value);
    }

    ~Semaphore() {
        dispatch_release(sem);
    }

    inline void wait() {
        dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
    }

    inline void post() {
        dispatch_semaphore_signal(sem);
    }
};

#elif __linux__ // todo: ELIF LINUX

class Semaphore
{
    sem_t sem;

    Semaphore(const Semaphore& other) = delete;
    Semaphore& operator=(const Semaphore& other) = delete;

public:
    inline Semaphore(uint32_t value = 0)
    {
        sem_init(&sem, 0, value);
    }

    ~Semaphore()
    {
        sem_destroy(&sem);
    }

    inline void wait()
    {
        int rc = -1;
        do
        {
            rc = sem_wait(&sem);
        }
        while (rc == -1 && errno == EINTR);
    }

    inline void post() {
        sem_post(&sem);
    }
};

#else

class Semaphore
{
    std::counting_semaphore<256> sem;

    Semaphore(const Semaphore& other) = delete;
    Semaphore& operator=(const Semaphore& other) = delete;

public:
    inline Semaphore(std::ptrdiff_t value = 0)
        : sem{value}
    {
        // Do nothing
    }

    ~Semaphore()
    {
        // Do nothing
    }

    inline void wait()
    {
        sem.acquire();
    }

    inline void post() {
        sem.release();
    }
};

#endif // NOT APPLE

// ERROR HERE, NOT DEFINED
