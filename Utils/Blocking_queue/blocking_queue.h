#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H
#include <condition_variable>
#include <queue>
#include <iostream>

template <typename T>
class BlockingQueue{

  std::mutex m_mutex;
  std::condition_variable m_condition_variable;
  std::queue<T> m_queue;
  std::size_t m_size;

  public:

   explicit BlockingQueue(std::size_t size) : m_size(size) {}

  void push(const T& item) noexcept{
     std::unique_lock<std::mutex> lock(m_mutex);
     m_condition_variable.wait(lock, [&]{return m_queue.size() < m_size;});
     m_queue.push(item);
     lock.unlock();
     m_condition_variable.notify_one();
   }

   void push(T&& item) noexcept{
     std::unique_lock<std::mutex> lock(m_mutex);
     m_condition_variable.wait(lock, [&]{return m_queue.size() < m_size;});
     m_queue.push(item);
     lock.unlock();
     m_condition_variable.notify_one();
   }

   bool try_push(T&& item) {
     if(m_queue.size() < m_size) {
       m_queue.push(item);
     }
     return m_queue.size() < m_size;
   }

   void pop() noexcept{
     std::unique_lock<std::mutex> lock(m_mutex);
     m_condition_variable.wait(lock, [&]{return !m_queue.empty();});
     m_queue.pop();
     lock.unlock();
     m_condition_variable.notify_one();
   }

   bool try_pop() {
     if(!m_queue.empty()) {
       m_queue.pop();
     }
     return !m_queue.empty();
   }

   ~BlockingQueue() {
     std::unique_lock<std::mutex> lock(m_mutex);
     m_condition_variable.notify_all();
   }
};

#endif //BLOCKING_QUEUE_H
