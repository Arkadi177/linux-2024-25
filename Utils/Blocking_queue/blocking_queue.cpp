#include "blocking_queue.h"

template<class T>
BlockingQueue<T>::BlockingQueue(int size) : size_(size) {}


template<class T>
void BlockingQueue<T>::push(const T& item) {
  std::unique_lock<std::mutex> lock(mutex_);
  condition_variable.wait(lock, [&]{return queue_.size() < size_;});
  queue_.push(item);
  std::cout << "Producing " << std::endl;
  std::cout << "Buffer size after consuming: " << queue_.size() << std::endl << std::endl;
  lock.unlock();
  condition_variable.notify_one();
}

template <typename T>
void BlockingQueue<T>::pop(){
  std::unique_lock<std::mutex> lock(mutex_);
  condition_variable.wait(lock, [&]{return !queue_.empty();});
  queue_.pop();
  std::cout << "Consuming " << std::endl;
  std::cout << "Buffer size after producing: " << queue_.size() << std::endl << std::endl;
  lock.unlock();
  condition_variable.notify_one();
}

template class BlockingQueue<int>;