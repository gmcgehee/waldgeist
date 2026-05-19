#include "threadpool.hpp"

#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <condition_variable>
#include <future>

// large portions of code taken from source cited in threadpool.hpp

void ThreadPool::worker()
{
    while (true)
    {
        std::function<void()> curr_task;

        {
            std::unique_lock<std::mutex> lock(mtx);

            cv.wait(lock, [this]()
                    { return stop || !tasks.empty(); });

            if (stop && tasks.empty())
                break;
            if (tasks.empty())
                continue;

            curr_task = tasks.front();
            tasks.pop();
        }

        curr_task();
    }
}

ThreadPool::ThreadPool(size_t thread_count)
{
    stop = false;
    for (auto i{0}; i < thread_count; i++)
    {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }

    cv.notify_all();
    for (auto &worker : workers)
    {
        worker.join();
    }
}

template<typename F, typename... Args>
inline auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<decltype(f(args...))> {
    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    auto encapsulated_ptr =
        std::make_shared<std::packaged_task<decltype(f(args...))()>>
        (func);

    std::future<std::result_of_t<F(Args...)>> future_object = encapsulated_ptr->get_future();
    {
        std::unique_lock<std::mutex> lock(mutex);
        queue.emplace([encapsulated_ptr]() {
            (*encapsulated_ptr)(); // execute the fx
            });
    }
    cv.notify_one();
    return future_object;
}
