#include "LinuxThread.h"
#include <cassert>
#include <iostream>

// Simple function to be executed by threads
void* thread_function(void* arg) {
    int* value = static_cast<int*>(arg);
    (*value)++;
    std::cout << "Thread function executed, value: " << *value << std::endl;
    return nullptr;
}

void test_linux_thread_creation() {
    // Test thread creation and state
    LinuxThread thread;
    assert(thread.get_status() == "NOT_CREATED" && "Thread should be in NOT_CREATED state");

    // Start the thread
    int value = 0;
    thread.start(thread_function, &value);
    assert(thread.get_status() == "STARTED" && "Thread should be in STARTED state after start");

    // Join the thread and check the status
    thread.join();
    assert(thread.get_status() == "DETACHED" && "Thread should be in DETACHED state after join");

    std::cout << "test_linux_thread_creation passed\n";
}

void test_linux_thread_multiple_join() {
    LinuxThread thread1, thread2;

    // Start two threads and check their status
    int value1 = 0, value2 = 0;
    thread1.start(thread_function, &value1);
    thread2.start(thread_function, &value2);

    assert(thread1.get_status() == "STARTED" && "Thread 1 should be in STARTED state");
    assert(thread2.get_status() == "STARTED" && "Thread 2 should be in STARTED state");

    // Join both threads
    thread1.join();
    thread2.join();

    assert(thread1.get_status() == "DETACHED" && "Thread 1 should be in DETACHED state after join");
    assert(thread2.get_status() == "DETACHED" && "Thread 2 should be in DETACHED state after join");

    std::cout << "test_linux_thread_multiple_join passed\n";
}

void test_linux_thread_detach() {
    LinuxThread thread;

    // Start and detach the thread
    int value = 0;
    thread.start(thread_function, &value);
    assert(thread.get_status() == "STARTED" && "Thread should be in STARTED state");

    thread.join(); // Joining the thread will change its status to DETACHED
    assert(thread.get_status() == "DETACHED" && "Thread should be in DETACHED state after join");

    std::cout << "test_linux_thread_detach passed\n";
}

void test_linux_thread_no_join_or_detach() {
    LinuxThread thread;
    assert(thread.get_status() == "NOT_CREATED" && "Thread should not be created yet");

    try {
        thread.join();  // Should throw an exception because thread is not started
    } catch (const std::exception& e) {
        std::cout << "Caught exception as expected: " << e.what() << "\n";
    }

    try {
        thread.join();  // Should throw an exception again because thread wasn't started
    } catch (const std::exception& e) {
        std::cout << "Caught exception as expected: " << e.what() << "\n";
    }

    std::cout << "test_linux_thread_no_join_or_detach passed\n";
}

void test_linux_thread_move() {
    LinuxThread thread1;

    // Start the thread
    int value = 0;
    thread1.start(thread_function, &value);

    LinuxThread thread2 = std::move(thread1);  // Move thread1 to thread2

    assert(thread2.get_status() == "STARTED" && "Moved thread should be in STARTED state");
    assert(thread1.get_status() == "DETACHED" && "Original thread should be in DETACHED state after move");

    thread2.join();  // Join moved thread

    std::cout << "test_linux_thread_move passed\n";
}

int main() {
  test_linux_thread_creation();
  test_linux_thread_multiple_join();
  test_linux_thread_detach();
  test_linux_thread_no_join_or_detach();
  test_linux_thread_move();

  std::cout << "All tests for LinuxThread passed!\n";
  return 0;
}