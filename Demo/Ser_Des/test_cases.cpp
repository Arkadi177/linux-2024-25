#include "ser_des.h"
#include <chrono>
#include <iostream>
#include <cassert>

struct TestObject {
  int id;
  double value;
  char name[256];
};

void runTest() {
  TestObject original{42, 3.14159, "Serialized Object"};
  TestObject recovered{};

  // Serialize
  auto start = std::chrono::high_resolution_clock::now();
  serialize(original);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Serialization time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
            << " microseconds" << std::endl;

  // Deserialize
  start = std::chrono::high_resolution_clock::now();
  deserialize(recovered);
  end = std::chrono::high_resolution_clock::now();
  std::cout << "Deserialization time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
            << " microseconds" << std::endl;

  // Validate the results
  assert(original.id == recovered.id);
  assert(original.value == recovered.value);
  assert(std::strcmp(original.name, recovered.name) == 0);
  std::cout << "Test passed!" << std::endl;
}

int main() {
  runTest();
  return 0;
}
