#ifndef LINUXTHREAD_H
#define LINUXTHREAD_H
#include <complex>
#include <functional>
#include <pthread.h>
#include <cstring>
#include <iostream>
#include <stdexcept>

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
    explicit LinuxThread() : thread{}, return_status(nullptr), status(NOT_CREATED) {}

    void start(void* (*function)(void*), void* arg) {
        if (pthread_create(&thread, nullptr, function, arg) != 0) {
            throw std::runtime_error(std::strerror(errno));
        }
        status = STARTED;
    }

    [[nodiscard]] pthread_t self() const {
        return thread;
    }

    void join() {
        if (pthread_join(thread, &return_status) != 0) {
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
        switch (status) {
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

    LinuxThread(LinuxThread&& other) noexcept
        : thread(other.thread), return_status(other.return_status), status(other.status) {
        other.thread = pthread_t{}; /
        other.status = DETACHED;
        other.return_status = nullptr;
    }

    LinuxThread& operator=(LinuxThread&& other) noexcept {
        if (this != &other) {
            if (status == STARTED) {
                pthread_detach(thread);
            }

            thread = other.thread;
            return_status = other.return_status;
            status = other.status;

            other.thread = pthread_t{};
            other.status = DETACHED;
            other.return_status = nullptr;
        }
        return *this;
    }

    LinuxThread(const LinuxThread&) = delete;
    LinuxThread& operator=(const LinuxThread&) = delete;
};

#endif //LINUXTHREAD_H
