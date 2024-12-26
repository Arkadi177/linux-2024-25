#include "my_allocator.h"

FixedBufferAllocator::FixedBufferAllocator(void* buf, std::size_t bufSize)
    : buffer(static_cast<std::byte*>(buf)), size(bufSize), freeList(nullptr) {
  freeList = reinterpret_cast<Block*>(buffer);
  freeList->block_size = bufSize - sizeof(Block);
  freeList->isFree = true;
  freeList->next = nullptr;
}

void* FixedBufferAllocator::allocate(std::size_t chunkSize) {
  if (chunkSize == 0 || chunkSize > size - sizeof(Block)) {
    throw std::bad_alloc();
  }

  Block* current = freeList;
  Block* prev = nullptr;

  while (current != nullptr) {
    if (current->isFree == true && current->block_size >= chunkSize) {
      if (current->block_size > chunkSize + sizeof(Block)) {
        Block* newBlock = reinterpret_cast<Block*>(
            reinterpret_cast<std::byte*>(current) + sizeof(Block) + chunkSize
        );
        newBlock->block_size = current->block_size - chunkSize - sizeof(Block);
        newBlock->isFree = true;
        newBlock->next = current->next;
        current->block_size = chunkSize;
        current->next = newBlock;
      }
      current->isFree = false;
      if (prev != nullptr) {
        prev->next = current->next;
      } else {
        freeList = current->next;
      }
      return reinterpret_cast<void*>(current + 1);
    }

    prev = current;
    current = current->next;
  }

  throw std::bad_alloc();
}

void FixedBufferAllocator::deallocate(void* ptr) {
  Block* block = reinterpret_cast<Block*>(static_cast<std::byte*>(ptr) - sizeof(Block));
  block->isFree = true;

  block->next = freeList;
  freeList = block;
}
