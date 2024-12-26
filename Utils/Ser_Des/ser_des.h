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

    explicit file(const std::filesystem::path& path , int flags , int mode);
    ~file();
};

struct file_map {
    std::byte* data;
    size_t size;

    file_map(void* address , size_t size , int prototype , int flags , int fd , off_t offset);
    ~file_map();

    [[nodiscard]] std::byte* get() const;
};

constexpr size_t block_size = 4096;
constexpr int serializationMagic = 564982;
constexpr auto size_int = sizeof(int);

template <class T>
void serialize(T& object , std::filesystem::path& path);

template <class T>
void deserialize(T& object , std::filesystem::path& path);

#endif // SER_DES_H
