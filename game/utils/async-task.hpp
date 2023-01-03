#pragma once

#include <thread>
#include "semaphores.h"

class AsyncTask {

    Semaphore m_worker_semaphore { 0 };
    Semaphore m_main_semaphore { 0 };

    std::thread m_thread;
    std::atomic<bool> m_destroyed { false };
public:
    AsyncTask(): m_thread(&AsyncTask::async_loop_entry, this) {}
    virtual ~AsyncTask();

    void run_async();
    void wait();

    virtual void async_task();
    void async_loop();
    static void async_loop_entry(AsyncTask* task);
};