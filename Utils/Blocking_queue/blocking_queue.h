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
  bool m_merar_ara;

  public:

   explicit BlockingQueue(std::size_t size) : m_size(size) , m_merar_ara(false){}

  void push(const T& item) noexcept{
     std::unique_lock<std::mutex> lock(m_mutex);
     m_condition_variable.wait(lock, [&]{return m_queue.size() < m_size;});
     if(m_merar_ara) {
       return;
     }
     m_queue.push(item);
     lock.unlock();
     m_condition_variable.notify_one();
   }

   void push(T&& item) noexcept{
     std::unique_lock<std::mutex> lock(m_mutex);
     m_condition_variable.wait(lock, [&]{return m_queue.size() < m_size;});
     if(m_merar_ara) {
       return;
     }
     m_queue.push(std::move(item));
     lock.unlock();
     m_condition_variable.notify_one();
   }

   bool try_push(const T& item) {
     std::unique_lock<std::mutex> lock(m_mutex);
     if(m_merar_ara) {
       return false;
     }
     else if(m_queue.size() < m_size) {
       m_queue.push(item);
     }
     lock.unlock();
     return m_queue.size() < m_size;
   }

   bool try_push(T&& item) {
      std::unique_lock<std::mutex> lock(m_mutex);
      if(m_merar_ara) {
        return false;
      }
      else if(m_queue.size() < m_size) {
        m_queue.push(std::move(item));
      }
      lock.unlock();
      return m_queue.size() < m_size;
    }

   void pop() noexcept{
     std::unique_lock<std::mutex> lock(m_mutex);
     m_condition_variable.wait(lock, [&]{return !m_queue.empty();});
     if(m_merar_ara) {
       return;
     }
     m_queue.pop();
     m_size--;
     lock.unlock();
     m_condition_variable.notify_one();
   }

   bool try_pop() {
     std::unique_lock<std::mutex> lock(m_mutex);
     if(m_merar_ara) {
       return false;
     }
     else if(!m_queue.empty()) {
       m_queue.pop();
     }
     m_size--;
     lock.unlock();
     return !m_queue.empty();
   }

   ~BlockingQueue() {
     std::unique_lock<std::mutex> lock(m_mutex);
     m_merar_ara = true;
     m_condition_variable.notify_all();
   }

};

#endif //BLOCKING_QUEUE_H
