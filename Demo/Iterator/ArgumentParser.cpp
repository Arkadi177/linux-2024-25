#include "ArgumentParser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
// Testing functions
void ArgParserTest1() {
  std::vector<std::string> args = {"program1", "-o",
                                   "-v", "source.txt", "dest.txt"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &arg: args) {
    argv.push_back(arg.data());
  }

  ArgumentParser parser(static_cast<int>(argv.size()), argv.data(), "ov:");

  std::cout << "Test 1:" << std::endl;
  for (const auto& arg : parser) {
    std::cout << arg.m_key.value_or("(positional)") << " "
              << arg.m_value.value_or("") << std::endl;
  }
  /** Should print:
   * o
   * v source.txt
   * (positional) dest.txt
   */

  std::cout << std::endl;
}

void ArgParserTest2() {
  std::vector<std::string> args = {"program2", "-f", "-o", "output.txt",
                                   "input.txt1", "input.txt2"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &arg: args) {
    argv.push_back(arg.data());
  }

  ArgumentParser parser(static_cast<int>(argv.size()), argv.data(), "fo:");

  std::cout << "Test 2:" << std::endl;
  for (auto arg : parser) {
    std::cout << arg.m_key.value_or("(positional)") << " "
              << arg.m_value.value_or("") << std::endl;
  }
  std::cout << std::endl;
  /** Should print:
   * f
   * o output.txt
   * (positional) input.txt1
   * (positional) input.txt2
   */
}

int main() {
  ArgParserTest2();
  ArgParserTest1();
  /** Should print:
   * o
   * v source.txt
   * (positional) dest.txt
   */

  /** Should print:
   * f
   * o output.txt
   * (positional) input.txt1
   * (positional) input.txt2
   */

  return 0;
}
