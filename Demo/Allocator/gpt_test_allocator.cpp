#include "my_allocator.h"
#include <vector>
#include <iostream>

int main() {
  constexpr std::size_t bufferSize = 1024;
  char buffer[bufferSize]; // Fixed-size buffer

  FixedBufferAllocator allocator(buffer, bufferSize);

  try {
    void* chunk1 = allocator.allocate(128);
    std::cout << "Allocated 128 bytes at: " << chunk1 << "\n";

    void* chunk2 = allocator.allocate(256);
    std::cout << "Allocated 256 bytes at: " << chunk2 << "\n";

    allocator.deallocate(chunk1);
    std::cout << "Deallocated 128 bytes from: " << chunk1 << "\n";

    void* chunk3 = allocator.allocate(64);
    std::cout << "Allocated 64 bytes at: " << chunk3 << "\n";

  } catch (const std::bad_alloc& e) {
    std::cerr << "Allocation error: " << e.what() << "\n";
  }

  return 0;
}


