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

    explicit file(const std::string& path , int flags , int mode)
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
    file_map(void* addres , size_t size , int prototype , int flags , int fd , off_t offset): size(size) {
        auto ptr = mmap(addres , size , prototype , flags , fd , offset);
        if(ptr == MAP_FAILED) {
            throw std::runtime_error(std::strerror(errno));
        }
        data = static_cast<std::byte*>(ptr);
    }

    ~file_map() {
        munmap(data, size);
    }

    std::byte* operator*() const{
        return data;
    }
};

constexpr size_t block_size = 4096;
constexpr int serializationMagic = 564982;
template <class T>

void serialize(T& object , std::string& path) {
    try{
        static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");

        file my_file(path , O_RDWR | O_CREAT , 0666);

        if (ftruncate(my_file.fd, sizeof(T) + sizeof(int)) == -1) {
            std::cerr << "Error resizing file: " << std::strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

        write(my_file.fd , &serializationMagic , sizeof(serializationMagic));

        file_map map(nullptr, block_size, PROT_WRITE, MAP_SHARED, my_file.fd, 0);

        std::memcpy(*map + sizeof(int), &object, sizeof(T));
        munmap(*map + sizeof(int), block_size);

    }catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

template <class T>
void deserialize(T& object , std::string& path) {
    try{
        if(!std::is_trivially_copyable<T>::value) {
            std::cerr << "Object is not trivially copyable! " << std::endl;
            exit(1);
        }

        file my_file(path , O_RDWR | O_CREAT , 0666);

        int number = 0;

        lseek(my_file.fd, 0, SEEK_SET);
        read(my_file.fd, &number, sizeof(int));

        if(number != serializationMagic) {
            std::cerr << "Error reading file: " << std::endl;
            exit(EXIT_FAILURE);
        }

        file_map map(nullptr, block_size, PROT_READ ,MAP_SHARED, my_file.fd, 0);

        std::memcpy(&object, *map + sizeof(int), sizeof(T));
        munmap(*map + sizeof(int), block_size);

    }catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}
#endif
