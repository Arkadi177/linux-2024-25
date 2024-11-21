#include <string>
#include "ser_des.h"

struct NonTrivialType {
  NonTrivialType(int x) : value(x) {}
  NonTrivialType(const NonTrivialType&) = default;
  ~NonTrivialType() {}  // Non-trivial destructor makes it non-trivially copyable
  int value;
};

int main() {
  std::string path = "test_non_trivial.bin";
  NonTrivialType obj(42);

  serialize(obj, path);

  return 0;
}
