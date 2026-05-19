#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <condition_variable>

// basic threadpool template from https://dev.to/ish4n10/making-a-thread-pool-in-c-from-scratch-bnm

class ThreadPool
{
private:
    std::vector<std::thread> workers;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<std::function<void()>> tasks;
    bool stop;

    void worker();

public:
    ThreadPool(size_t thread_count = std::thread::hardware_concurrency());
    ~ThreadPool();

    template<typename T, typename... Args>
    auto enqueue();

    
    ThreadPool(ThreadPool&) = delete;
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

};