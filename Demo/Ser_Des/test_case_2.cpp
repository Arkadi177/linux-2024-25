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

class CustomObject {
public:
    CustomObject() = default;
    CustomObject(int a, float b, const std::string& s) : int_val(a), float_val(b) {
        std::strncpy(str_val, s.c_str(), sizeof(str_val));
        str_val[sizeof(str_val) - 1] = '\0'; // Null-terminate the string
    }

    bool operator==(const CustomObject& other) const {
        return int_val == other.int_val &&
               float_val == other.float_val &&
               std::strcmp(str_val, other.str_val) == 0;
    }

private:
    int int_val;
    float float_val;
    char str_val[50];

    friend void serialize<CustomObject>(CustomObject&, std::string&);
    friend void deserialize<CustomObject>(CustomObject&, std::string&);
};

int main() {
    std::string path = "test_file_2.bin";
    CustomObject original(7, 2.718f, "Hello, World!");
    CustomObject deserialized;

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
    assert(original == deserialized && "CustomObject does not match!");

    std::cout << "Test 2 passed: CustomObject serialization and deserialization are correct!" << std::endl;
    std::cout << "Serialization took " << serialize_duration << " microseconds.\n";
    std::cout << "Deserialization took " << deserialize_duration << " microseconds.\n";

    return 0;
}
