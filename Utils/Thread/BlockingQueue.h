#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H
#include <condition_variable>
#include <queue>
#include <iostream>

std::vector<bool> m_dead;
int i = 0;

template <typename T>
class BlockingQueue{

  std::mutex m_mutex;
  std::condition_variable m_condition_variable;
  std::queue<T> m_queue;
  std::size_t m_size;
  int m_index;

  public:

   explicit BlockingQueue(std::size_t size)
   : m_size(size) , m_index(i) {
     m_dead.push_back(false);
   }

    void push(const T& item) {
       std::unique_lock<std::mutex> lock(m_mutex);
       m_condition_variable.wait(lock, [&]{return m_queue.size() < m_size;});
       if(m_dead[m_index]) {
         return;
       }
       m_queue.push(item);
       lock.unlock();
       m_condition_variable.notify_one();
    }

    void push(T&& item) {
       std::unique_lock<std::mutex> lock(m_mutex);
       m_condition_variable.wait(lock, [&]{return m_queue.size() < m_size;});
       if(m_dead[m_index]) {
         return;
       }
       m_queue.push(std::move(item));
       lock.unlock();
       m_condition_variable.notify_one();
    }

    bool try_push(const T& item) {
       std::unique_lock<std::mutex> lock(m_mutex);
       if(m_dead[m_index]) {
         return false;
       }
       else if(m_queue.size() < m_size) {
         m_queue.push(item);
       }
       return m_queue.size() < m_size;
    }

    bool try_push(T&& item) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if(m_dead[m_index]) {
          return false;
        }
        else if(m_queue.size() < m_size) {
          m_queue.push(std::move(item));
        }
        return m_queue.size() < m_size;
    }

    T pop() noexcept{
       std::unique_lock<std::mutex> lock(m_mutex);
       T item = {};
       m_condition_variable.wait(lock , [&]{return !m_queue.empty();});
       if(m_dead[m_index]) {
         return item;
       }
       item = std::move(m_queue.front());
       m_queue.pop();
       lock.unlock();
       m_condition_variable.notify_one();
       return item;
    }

    bool try_pop(T& item) {
       std::unique_lock<std::mutex> lock(m_mutex);
       if(m_dead[m_index] || m_queue.empty()) {
         return false;
       }
       item = std::move(m_queue.front());
       m_queue.pop();
       return true;
    }

    ~BlockingQueue() {
       std::unique_lock<std::mutex> lock(m_mutex);
       m_dead[m_index] = true;
       i++;
       m_condition_variable.notify_all();
    }

};

#endif //BLOCKING_QUEUE_H