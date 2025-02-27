#include <iostream>
#include "ThreadPool.h"

int main() {
  ThreadPool pool;  // Create thread pool with 4 threads

  // Enqueue some tasks
  auto future1 = pool.enqueue([](int x, int y) { return x + y; }, 3, 7);
  auto future2 = pool.enqueue([]() { return std::string("Hello from thread pool!"); });
  std::cout << "Result 1: " << future1.get() << std::endl;  // Output: 10
  std::cout << "Result 2: " << future2.get() << std::endl;  // Output: Hello from thread pool!

  for (int i = 0; i < 100; ++i)
  {
    auto future3 = pool.enqueue([](int x, int y) { return x + y; }, i, i);
    std::cout << "Result 3: " << future3.get() << std::endl;
  }

  return 0;
}
