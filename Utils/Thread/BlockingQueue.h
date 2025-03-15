#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <condition_variable>
#include <queue>
#include <iostream>
#include <optional>

template <typename T>
class BlockingQueue
{
    std::mutex& m_mutex;
    std::condition_variable& m_condition_variable;
    std::queue<T> m_queue;
    std::size_t m_size = 0;

public:
    BlockingQueue(std::mutex& mutex, std::condition_variable& condition_variable, std::size_t size)
        : m_mutex(mutex) ,
          m_condition_variable(condition_variable) ,
          m_size(size){}

    void push(const T& item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition_variable.wait(lock, [&] { return m_queue.size() < m_size; });
        m_queue.push(item);
        m_condition_variable.notify_one();
     }

    void push(T&& item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition_variable.wait(lock, [&] { return m_queue.size() < m_size; });
        m_queue.push(std::move(item));
        m_condition_variable.notify_one();
    }

    bool try_push(const T& item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.size() < m_size)
        {
           m_queue.push(item);
        }
        return m_queue.size() < m_size;
    }

    bool try_push(T&& item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.size() < m_size)
        {
           m_queue.push(std::move(item));
        }
        return m_queue.size() < m_size;
    }

    std::optional<T> pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition_variable.wait(lock, [&] { return !m_queue.empty(); });

        std::optional<T> result;
        if constexpr (std::is_nothrow_move_constructible_v<T>) {
            result = std::move(m_queue.front());
        } else {
            result = m_queue.front();
        }

        m_queue.pop();
        m_condition_variable.notify_one();
        return result;
    }

    bool try_pop(T& item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty()) {
            return false;
        }
        item = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

};

#endif // BLOCKING_QUEUE_H
