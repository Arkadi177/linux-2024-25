#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <chrono> // For benchmarking
#include "ser_des.h"
// Include serialize and deserialize code here.

constexpr size_t array_size = 10;

void test3() {
  std::string path = "test_file_3.bin";
  int original[array_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int deserialized[array_size] = {0};

  // Benchmark serialization
  auto start_serialize = std::chrono::high_resolution_clock::now();
  serialize(original, path);
  auto end_serialize = std::chrono::high_resolution_clock::now();
  auto serialize_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_serialize - start_serialize).count();

  // Benchmark deserialization
  auto start_deserialize = std::chrono::high_resolution_clock::now();
  deserialize(deserialized, path);
  auto end_deserialize = std::chrono::high_resolution_clock::now();
  auto deserialize_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_deserialize - start_deserialize).count();

  // Assertions to check correctness
  for (size_t i = 0; i < array_size; ++i) {
    assert(original[i] == deserialized[i] && "Array elements do not match!");
  }

  std::cout << "Test 3 passed: Array serialization and deserialization are correct!" << std::endl;
  std::cout << "Serialization took " << serialize_duration << " microseconds.\n";
  std::cout << "Deserialization took " << deserialize_duration << " microseconds.\n";

}
