#ifndef LINUXMUTEXGUARD_H
#define LINUXMUTEXGUARD_H

#include "LinuxMutex.h"

class LinuxMutexGuard {
  LinuxMutex& mutex;

public:
  explicit LinuxMutexGuard(LinuxMutex& mtx) : mutex(mtx) {
    mutex.lock();
  }

  ~LinuxMutexGuard() {
    mutex.unlock();
  }

  LinuxMutexGuard(const LinuxMutexGuard&) = delete;
  LinuxMutexGuard& operator=(const LinuxMutexGuard&) = delete;
};

#endif // LINUXMUTEX_H