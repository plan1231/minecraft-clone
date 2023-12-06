//
// Created by Phil Lan on 2023-12-05.
//

#include "ThreadPool.h"

#include <thread>

void ThreadPool::start() {
    const uint32_t numThreads = std::thread::hardware_concurrency();
    for(uint32_t i = 0; i < numThreads; i++) {
        threads.emplace_back(&ThreadPool::threadLoop, this);
    }
}

void ThreadPool::queueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        jobs.push(job);
        ++activeJobs;
    }
    mutexCondition.notify_one();
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        shouldTerminate = true;
    }
    mutexCondition.notify_all();
    for(auto &t : threads) {
        t.join();
    }
    threads.clear();
}

bool ThreadPool::busy() {
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(queueMutex);
    jobsDoneCondition.wait(lock, [this](){ return jobs.empty() && activeJobs == 0; });
}

void ThreadPool::threadLoop() {
    while(true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            mutexCondition.wait(lock, [this] {
                return !jobs.empty() || shouldTerminate;
            });
            if(shouldTerminate) return;
            job = jobs.front();
            jobs.pop();
        }
        job();
        {
            --activeJobs;
            std::unique_lock<std::mutex> lock(queueMutex);
            if(activeJobs == 0) {
                jobsDoneCondition.notify_all();
            }
        }
    }
}
