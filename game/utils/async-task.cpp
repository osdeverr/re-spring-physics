//
// Created by Артем on 06.02.2022.
//

#include "async-task.hpp"

AsyncTask::~AsyncTask() {
    m_destroyed = true;
    run_async();
    m_thread.join();
}

void AsyncTask::run_async() {
    m_worker_semaphore.post();
}

void AsyncTask::wait() {
    m_main_semaphore.wait();
}

void AsyncTask::async_task() {

}

void AsyncTask::async_loop() {
    while(!m_destroyed) {
        m_main_semaphore.post();
        m_worker_semaphore.wait();
        if(m_destroyed) break;

        async_task();
        atomic_thread_fence(std::memory_order_release);
    }
}

void AsyncTask::async_loop_entry(AsyncTask* task) {
    task->async_loop();
}
