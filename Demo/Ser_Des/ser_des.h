#ifndef SER_DES_H
#define SER_DES_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <cerrno>
#include <type_traits>

constexpr size_t block_size = 4096;

template <class T>
void serialize(T& object , std::string& path) {
    if(!std::is_trivially_copyable<T>::value) {
        std::cerr << "Object is not trivially copyable! " << std::endl;
        exit(1);
    }
    int fd = open(path.c_str(), O_RDWR | O_CREAT , 0666);
    if (fd == -1) {
        std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
        exit(1);
    }

    if (ftruncate(fd, sizeof(T)) == -1) {
        std::cerr << "Error resizing file: " << std::strerror(errno) << std::endl;
        exit(1);
    }

    void* ptr = mmap(nullptr, block_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Error mapping file: " << strerror(errno) << std::endl;
        close(fd);
        exit(1);
    }
    std::memcpy(ptr, &object, sizeof(T));
    munmap(ptr, block_size);
    close(fd);
}

template <class T>
void deserialize(T& object , std::string& path) {
    if(!std::is_trivially_copyable<T>::value) {
        std::cerr << "Object is not trivially copyable! " << std::endl;
        exit(1);
    }
    int fd = open(path.c_str() , O_RDWR | O_CREAT , 0666);
    if (fd == -1) {
        std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
        exit(1);
    }

    void* ptr = mmap(nullptr, block_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Error mapping file: " << std::strerror(errno) << std::endl;
        exit(1);
    }
    std::memcpy(&object, ptr, sizeof(T));
    munmap(ptr, block_size);
    close(fd);
}
#endif
