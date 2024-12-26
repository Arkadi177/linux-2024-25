#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include "do_command.h"

int main() {
  std::string command_to_do;
  getline(std::cin, command_to_do);
  do_command(command_to_do);
}