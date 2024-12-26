#pragma once
#include <cstddef> // For std::size_t
#include <stdexcept> // For std::bad_alloc

class FixedBufferAllocator {
private:
    struct Block {
        std::size_t block_size;
        bool isFree;
        Block* next;
    };

    std::byte* buffer;
    std::size_t size;
    Block* freeList;

public:
    FixedBufferAllocator(void* buf, std::size_t bufSize);
    void* allocate(std::size_t chunkSize);
    void deallocate(void* ptr);
};
