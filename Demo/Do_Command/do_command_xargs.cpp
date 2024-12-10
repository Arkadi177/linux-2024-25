#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include "do_command.h"

void xargs(int argc , char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: xargs [filename]" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string command_to_do;
  getline(std::cin, command_to_do);
  for (int i = 1; i < argc; i++) {
    command_to_do.append(std::string(argv[i]));
    command_to_do.push_back(' ');
  }
  do_command(command_to_do);
}

int main(int argc , char *argv[]) {
  xargs(argc, argv);
}