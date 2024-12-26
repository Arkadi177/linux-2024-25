#include "LinuxMutexGuard.h"
#include <thread>
#include <iostream>
#include <cassert>


void test_linux_mutex_guard_basic() {
  LinuxMutex mutex;

  {
    LinuxMutexGuard guard(mutex); // Lock acquired
    std::cout << "Mutex locked using LinuxMutexGuard\n";
  } // Lock released
  std::cout << "Mutex unlocked after LinuxMutexGuard goes out of scope\n";

  std::cout << "test_linux_mutex_guard_basic passed\n";
}

void test_linux_mutex_guard_multithread() {
  LinuxMutex mutex;
  int shared_counter = 0;

  auto increment = [&]() {
    for (int i = 0; i < 100; ++i) {
      LinuxMutexGuard guard(mutex);
      ++shared_counter;
    }
  };

  std::thread t1(increment);
  std::thread t2(increment);

  t1.join();
  t2.join();

  assert(shared_counter == 200 && "Counter should be 200 after both threads finish");
  std::cout << "Shared counter: " << shared_counter << "\n";
  std::cout << "test_linux_mutex_guard_multithread passed\n";
}

int main() {
  test_linux_mutex_guard_basic();
  test_linux_mutex_guard_multithread();

  std::cout << "All tests for LinuxMutexGuard passed!\n";
  return 0;
}
