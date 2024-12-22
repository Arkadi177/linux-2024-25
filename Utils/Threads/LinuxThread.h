#ifndef LINUXTHREAD_H
#define LINUXTHREAD_H
#include <complex>
#include <functional>
#include <pthread.h>
#include <cstring>

enum Status {
  NOT_CREATED,
  STARTED,
  DETACHED
};

class LinuxThread {
  pthread_t thread;
  void* return_status;
  Status status;

  public:

    explicit LinuxThread() : thread{} , return_status(nullptr) , status(NOT_CREATED) {}

    void start(void*(*function)(void*) , void* arg) {
      if(pthread_create(&thread , nullptr , function , arg) != 0) {
        throw std::runtime_error(std::strerror(errno));
      }
      status = STARTED;
    }

    [[nodiscard]] pthread_t self() const {
      return thread;
    }

    void join() {
      if(pthread_join(thread , &return_status) != 0) {
        throw std::runtime_error(std::strerror(errno));
      }
      status = DETACHED;
    }

    ~LinuxThread() {
        if (status == STARTED) {
          if (pthread_detach(thread) != 0) {
            std::cerr << "Warning: Failed to detach thread" << std::endl;
          }
        }
      status = DETACHED;
    }


    [[nodiscard]] std::string get_status() const {
      switch(status) {
        case NOT_CREATED:
          return "NOT_CREATED";
        case STARTED:
          return "STARTED";
        case DETACHED:
          return "DETACHED";
        default:
          return "UNKNOWN";
      }
    }
};

#endif //LINUXTHREAD_H
