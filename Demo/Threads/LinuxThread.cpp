#include <iostream>
#include <unistd.h> // for sleep
#include "LinuxThread.h"

// Thread function example
void* printMessage(void* arg) {
  const char* message = static_cast<const char*>(arg);
  for (int i = 0; i < 5; ++i) {
    std::cout << message << " - Iteration: " << i + 1 << std::endl;
    sleep(1); // Simulate work
  }
  return nullptr;
}

int main() {
  try {
    LinuxThread thread;

    // Start thread
    const char* message = "Hello from thread!";
    std::cout << "Starting thread..." << std::endl;
    thread.start(printMessage, (void*)message);

    // Display thread status
    std::cout << "Thread status: " << thread.get_status() << std::endl;

    // Wait for thread to finish
    thread.join();
    std::cout << "Thread has finished execution." << std::endl;

    // Display thread status after joining
    std::cout << "Thread status: " << thread.get_status() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
