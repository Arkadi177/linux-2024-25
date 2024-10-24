#include "ArgumentParser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
// Testing functions
void ArgParserTest1() {
  std::vector<std::string> args = {"program", "-o",
                                   "-v", "source.txt", "dest.txt"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &arg: args) {
    argv.push_back(arg.data());
  }

  ArgumentParser parser(static_cast<int>(argv.size()), argv.data(), "ov:");

  std::cout << "Test 1:\n";
  for (const auto& arg : parser) {
    std::cout << arg.m_key.value_or("(positional)") << " "
              << arg.m_value.value_or("") << "\n";
  }
  /** Should print:
   * o
   * v source.txt
   * (positional) dest.txt
   */

  std::cout << "\n";
}

void ArgParserTest2() {
  std::vector<std::string> args = {"program", "-f", "-o", "output.txt",
                                   "input.txt1", "input.txt2"};
  std::vector<char *> argv;
  argv.reserve(args.size());
  for (auto &arg: args) {
    argv.push_back(arg.data());
  }

  ArgumentParser parser(static_cast<int>(argv.size()), argv.data(), "fo:");

  std::cout << "Test 2:\n";
  for (auto arg : parser) {
    std::cout << arg.m_key.value_or("(positional)") << " "
              << arg.m_value.value_or("") << "\n";
  }
  std::cout << "\n";
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

  return 0;
}
