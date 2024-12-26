#ifndef LINUXMUTEX_H
#define LINUXMUTEX_H

#include <pthread.h>
#include <stdexcept>
#include <cstring>
#include <iostream>

class LinuxMutex {
  pthread_mutex_t mutex;

public:
  // Constructor
  LinuxMutex() {
    if (pthread_mutex_init(&mutex, nullptr) != 0) {
      throw std::runtime_error("Failed to initialize mutex: " + std::string(std::strerror(errno)));
    }
  }

  // Destructor
  ~LinuxMutex() {
    if (pthread_mutex_destroy(&mutex) != 0) {
      std::cerr << "Warning: Failed to destroy mutex" << std::endl;
    }
  }

  // Lock the mutex
  void lock() {
    if (pthread_mutex_lock(&mutex) != 0) {
      throw std::runtime_error("Failed to lock mutex: " + std::string(std::strerror(errno)));
    }
  }

  // Unlock the mutex
  void unlock() {
    if (pthread_mutex_unlock(&mutex) != 0) {
      throw std::runtime_error("Failed to unlock mutex: " + std::string(std::strerror(errno)));
    }
  }

  // Try to lock the mutex
  bool try_lock() {
    int result = pthread_mutex_trylock(&mutex);
    if (result == 0) {
      return true; // Lock acquired
    } else if (result == EBUSY) {
      return false; // Mutex already locked
    } else {
      throw std::runtime_error("Failed to try_lock mutex: " + std::string(std::strerror(errno)));
    }
  }
};

#endif