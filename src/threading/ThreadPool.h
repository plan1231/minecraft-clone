//
// Created by Phil Lan on 2023-12-05.
//

#pragma once
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>
class ThreadPool {
public:
    void start();
    void queueJob(const std::function<void()> &job);
    void stop();
    bool busy();
    void waitForCompletion();
private:
    void threadLoop();

    bool shouldTerminate = false;
    std::mutex queueMutex;
    std::condition_variable mutexCondition;
    std::condition_variable jobsDoneCondition;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
    std::atomic<int> activeJobs = 0;
};
