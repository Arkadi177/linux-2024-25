#ifndef SER_DES_H
#define SER_DES_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <cerrno>

template <class T>
void serialize(T& object) {
    int fd = open("/home/arkadi/linux-2024-25/Demo/Ser_Des/map.txt", O_RDWR);
    if (fd == -1) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
        exit(1);
    }

    if (ftruncate(fd, 4096) == -1) {
        std::cerr << "Error resizing file: " << strerror(errno) << std::endl;
        close(fd);
        exit(1);
    }

    void* ptr = mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Error mapping file: " << strerror(errno) << std::endl;
        close(fd);
        exit(1);
    }
    std::memcpy(ptr, &object, sizeof(T));
    munmap(ptr, 4096);
    close(fd);
}

template <class T>
void deserialize(T& object) {
    int fd = open("/home/arkadi/linux-2024-25/Demo/Ser_Des/map.txt", O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
        exit(1);
    }

    void* ptr = mmap(nullptr, 4096, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Error mapping file: " << strerror(errno) << std::endl;
        close(fd);
        exit(1);
    }
    std::memcpy(&object, ptr, sizeof(T));
    munmap(ptr, 4096);
    close(fd);
}
#endif
