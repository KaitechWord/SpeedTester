#include "Threadpool.h"

ThreadPool::ThreadPool(int threadsNum)
    : threadsNum(threadsNum)
{}

void ThreadPool::start() {
    for (int i = 0; i < threadsNum; ++i) {
        threads.emplace_back(std::thread(&ThreadPool::threadLoop, this));
    }
}

// To use it: thread_pool->QueueJob([] { /* ... */ });
void ThreadPool::queueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queueM);
        jobs.push(job);
    }
    mutexCond.notify_one();
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(this->queueM);
        this->shouldTerminate = true;
    }
    this->mutexCond.notify_all();
    for (std::thread& activeThread : this->threads) {
        activeThread.join();
    }
    this->threads.clear();
}

bool ThreadPool::busy() {
    bool poolBusy;
    {
        std::unique_lock<std::mutex> lock(this->queueM);
        poolBusy = !this->jobs.empty();
    }
    return poolBusy;
}

void ThreadPool::threadLoop() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(this->queueM);
            this->mutexCond.wait(lock, [this] {
                return !this->jobs.empty() || this->shouldTerminate;
                });
            if (this->shouldTerminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}

int ThreadPool::getThreadsNum() {
    return this->threadsNum;
}