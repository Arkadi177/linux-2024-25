#pragma once

#include "BlockingQueue.h"
#include <type_traits>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <tuple>

class ThreadPool
{
    template <class F, class... Args>
    using return_type = typename std::invoke_result_t<F, Args...>;

    std::vector<std::thread> m_threads;
    BlockingQueue<std::function<void()>>* m_tasks;
    std::atomic<bool> m_stop;
    std::atomic<std::size_t> m_thread_count;

public:
    explicit ThreadPool()
        : m_stop(false),
          m_thread_count(std::thread::hardware_concurrency()),
          m_tasks(new BlockingQueue<std::function<void()>>())
    {
        m_tasks->set_size(m_thread_count);
        for (size_t i = 0; i < m_thread_count; i++)
        {
            m_threads.emplace_back([this]
            {
                while (!m_stop)
                {
                    auto task = m_tasks->pop();
                    if (task.value())
                    {
                        task.value()();
                    }
                    else
                    {
                        m_stop = true;
                    }
                }
            });
        }
    }

    ~ThreadPool()
    {
        m_stop = true;
        for (int i = 0; i < m_thread_count; i++)
        {
            m_tasks->push(nullptr);
        }
        for (auto& t : m_threads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
        operator delete(m_tasks);
    }

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<return_type<F, Args...>>
    {
        auto promise = std::make_shared<std::promise<return_type<F, Args...>>>();
        auto future = promise->get_future();

        auto task = [promise, f = std::forward<F>(f), args = std::make_tuple(std::forward<Args>(args)...)]()
        {
            promise->set_value(std::apply(f, args));
        };

        m_tasks->push(std::move(task));
        return future;
    }
};
