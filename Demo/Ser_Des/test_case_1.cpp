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

struct Data {
  int id;
  double value;
};

void test1() {
  std::string path = "test_file_1.bin";
  Data original = {42, 3.14};
  Data deserialized;

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
  assert(original.id == deserialized.id && "ID does not match!");
  assert(original.value == deserialized.value && "Value does not match!");

  std::cout << "Test 1 passed: Struct serialization and deserialization are correct!" << std::endl;
  std::cout << "Serialization took " << serialize_duration << " microseconds.\n";
  std::cout << "Deserialization took " << deserialize_duration << " microseconds.\n";

}
