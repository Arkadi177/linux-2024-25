#ifndef SER_DES_H
#define SER_DES_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <cerrno>

#include <type_traits>

struct file {
    int fd;

    explicit file(const std::string& path)
        : fd(open(path.c_str() , O_RDWR | O_CREAT , 0666)){}

    void write_to_file(const int number)const {
        write(fd , &number , sizeof(number));
    }

    void read_from_file(int& number)const {
        lseek(fd , 0 , SEEK_SET);
        read(fd , &number, sizeof(number));
    }

    ~file() {
        close(fd);
    }
};

constexpr size_t block_size = 4096;
constexpr int magic_number = 564982;
template <class T>

void serialize(T& object , std::string& path) {

    if(!std::is_trivially_copyable<T>::value) {
        std::cerr << "Object is not trivially copyable! " << std::endl;
        exit(EXIT_FAILURE);
    }

    file my_file(path);

    if (my_file.fd == -1) {
        std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (ftruncate(my_file.fd, sizeof(T) + sizeof(int)) == -1) {
        std::cerr << "Error resizing file: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    my_file.write_to_file(magic_number);

    void* ptr = mmap(nullptr, block_size, PROT_WRITE, MAP_SHARED, my_file.fd, 0);

    if (ptr == MAP_FAILED) {
        std::cerr << "Error mapping file: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::memcpy(ptr + sizeof(int), &object, sizeof(T));
    munmap(ptr + sizeof(int), block_size);

}

template <class T>
void deserialize(T& object , std::string& path) {

    if(!std::is_trivially_copyable<T>::value) {
        std::cerr << "Object is not trivially copyable! " << std::endl;
        exit(1);
    }

    file my_file(path);

    if (my_file.fd == -1) {
        std::cerr << "Error opening file: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    int number = 0;
    my_file.read_from_file(number);

    if(number != magic_number) {
        std::cerr << "Error reading file: " << std::endl;
        exit(EXIT_FAILURE);
    }

    void* ptr = mmap(nullptr, block_size, PROT_READ, MAP_SHARED, my_file.fd, 0);

    if (ptr == MAP_FAILED) {
        std::cerr << "Error mapping file: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::memcpy(&object, ptr + sizeof(int), sizeof(T));
    munmap(ptr + sizeof(int), block_size);

}
#endif
