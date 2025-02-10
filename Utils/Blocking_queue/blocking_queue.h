#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H
#include <condition_variable>
#include <queue>
#include <thread>
#include <iostream>

template <typename T>
class BlockingQueue{
  std::mutex mutex_;
  std::condition_variable condition_variable;
  std::queue<T> queue_;
  int size_;
  public:
   BlockingQueue(int size);
   void push(const T& item);
   void pop();
};

#endif //BLOCKING_QUEUE_H
