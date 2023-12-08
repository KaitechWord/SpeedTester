#pragma once
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>
#include <thread>

class ThreadPool {
public:
    ThreadPool(int threadsNum);
    void start();
    void queueJob(const std::function<void()>& job);
    void stop();
    bool busy();
    int getThreadsNum();
private:
    int threadsNum;
    void threadLoop();
    bool shouldTerminate = false;           // Tells threads to stop looking for jobs
    std::mutex queueM;                  // Prevents data races to the job queue
    std::condition_variable mutexCond; // Allows threads to wait on new jobs or termination 
    std::condition_variable idleCond;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
};