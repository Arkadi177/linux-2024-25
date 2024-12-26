#include "LinuxMutex.h"
#include <thread>
#include <iostream>
#include <cassert>

void test_linux_mutex_lock_unlock() {
  LinuxMutex mutex;

  // Lock the mutex
  mutex.lock();
  std::cout << "Mutex locked successfully\n";

  // Unlock the mutex
  mutex.unlock();
  std::cout << "Mutex unlocked successfully\n";

  std::cout << "test_linux_mutex_lock_unlock passed\n";
}

void test_linux_mutex_try_lock() {
  LinuxMutex mutex;

  // Lock the mutex
  mutex.lock();
  assert(!mutex.try_lock() && "try_lock should fail when the mutex is already locked");
  std::cout << "Mutex try_lock failed as expected\n";

  // Unlock and try again
  mutex.unlock();
  assert(mutex.try_lock() && "try_lock should succeed when the mutex is not locked");
  mutex.unlock();
  std::cout << "Mutex try_lock succeeded as expected\n";

  std::cout << "test_linux_mutex_try_lock passed\n";
}

void test_linux_mutex_multithread() {
  LinuxMutex mutex;
  int shared_counter = 0;

  auto increment = [&]() {
    for (int i = 0; i < 100; ++i) {
      mutex.lock();
      ++shared_counter;
      mutex.unlock();
    }
  };

  std::thread t1(increment);
  std::thread t2(increment);

  t1.join();
  t2.join();

  assert(shared_counter == 200 && "Counter should be 200 after both threads finish");
  std::cout << "Shared counter: " << shared_counter << "\n";
  std::cout << "test_linux_mutex_multithread passed\n";
}

int main() {
  test_linux_mutex_lock_unlock();
  test_linux_mutex_try_lock();
  test_linux_mutex_multithread();

  std::cout << "All tests for LinuxMutex passed\n";
  return 0;
}