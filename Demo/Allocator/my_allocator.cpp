#include "my_allocator.h"
#include <vector>
#include <iostream>

int main() {
  try {
    std::vector<int, MyAllocator<int>> myVector;

    myVector.push_back(10);
    myVector.push_back(20);
    myVector.push_back(30);

    for (int val : myVector) {
      std::cout << val << " ";
    }
    std::cout << "\n";
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
