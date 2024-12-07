#if 0
#include <string>
#include "ser_des.h"

struct NonTrivialType {
  NonTrivialType(int x) : value(x) {}
  NonTrivialType(const NonTrivialType&) = default;
  ~NonTrivialType() {}  // Non-trivial destructor makes it non-trivially copyable
  int value;
};

void test4() {
  std::string path = "test_non_trivial.bin";
  NonTrivialType obj(42);

  serialize(obj, path);

} // if we comment out this it will throw assertion error
#endif
