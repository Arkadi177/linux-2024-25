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

    std::mutex m_mutex;
    std::condition_variable m_condition;

    std::vector<std::thread> m_threads;
    std::shared_ptr<BlockingQueue<std::function<void()>>> m_tasks;
    std::atomic<bool> m_stop;
    std::atomic<std::size_t> m_thread_count;

public:
    explicit ThreadPool()
        : m_stop(false),
          m_thread_count(std::thread::hardware_concurrency()),
          m_tasks(std::make_shared<BlockingQueue<std::function<void()>>>(m_mutex, m_condition, m_thread_count))
    {
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
            m_tasks->push([]{});
        }
        for (auto& t : m_threads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
    }

    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<return_type<F, Args...>>
    {
        using R = return_type<F, Args...>;
        auto promise = std::make_shared<std::promise<R>>();
        auto future = promise->get_future();

        auto task = [promise, f = std::forward<F>(f), args = std::make_tuple(std::forward<Args>(args)...)]() {
            try {
                if constexpr (std::is_void_v<R>) {
                    std::apply(f, args);
                    promise->set_value();
                } else {
                    promise->set_value(std::apply(f, args));
                }
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        };

        m_tasks->push(std::move(task));
        return future;
    }
};
