#ifndef SER_DES_H
#define SER_DES_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <cerrno>
#include <filesystem>
#include <type_traits>

struct file {
    int fd;

    explicit file(const std::filesystem::path& path , int flags , int mode)
        : fd(open(path.c_str() , flags , mode)) {
        if (fd == -1) {
            throw std::runtime_error(std::strerror(errno));
        }
    }

    ~file() {
        close(fd);
    }
};

struct file_map {
    std::byte* data;
    size_t size;
    file_map(void* address , size_t size , int prototype , int flags , int fd , off_t offset): size(size) {
        auto ptr = mmap(address , size , prototype , flags , fd , offset);
        if(ptr == MAP_FAILED) {
            throw std::runtime_error(std::strerror(errno));
        }
        data = static_cast<std::byte*>(ptr);
    }

    ~file_map() {
        munmap(data, size);
    }

    [[nodiscard]] std::byte* get() const {
        return data;
    }
};

constexpr size_t block_size = 4096;
constexpr int serializationMagic = 564982;
constexpr auto size_int = sizeof(int);

template <class T>
void serialize(T& object , std::filesystem::path& path) {
    static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");

    file my_file(path , O_RDWR | O_CREAT , 0666);

    if (ftruncate(my_file.fd, sizeof(T) + sizeof(int)) == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
    auto len = write(my_file.fd , &serializationMagic , sizeof(serializationMagic));

    if(len == -1) {
        throw std::runtime_error(std::strerror(errno));
    }
    file_map map(nullptr, block_size, PROT_WRITE, MAP_SHARED, my_file.fd, 0);

    std::memcpy(map.get() + size_int, &object, sizeof(T));

}

template <class T>
void deserialize(T& object , std::filesystem::path& path) {
    static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");

    file my_file(path , O_RDWR | O_CREAT , 0666);
    int number = 0;

    auto len = lseek(my_file.fd, 0, SEEK_SET);
    if(len == -1) {
        throw std::runtime_error(std::strerror(errno));
    }

    len = read(my_file.fd, &number, size_int);
    if(len == -1) {
        throw std::runtime_error(std::strerror(errno));
    }

    if(number != serializationMagic) {
        throw std::runtime_error(std::strerror(errno));
    }

    file_map map(nullptr, block_size, PROT_READ ,MAP_SHARED, my_file.fd, 0);
    std::memcpy(&object, map.get() + size_int, sizeof(T));

}
#endif
