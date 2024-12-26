#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <string>

int do_command(const std::string& command_to_do) {
  pid_t child = fork();

  if (child == 0) {
    execl("/bin/sh", "sh", "-c", command_to_do.c_str(), nullptr);
    return -1;
  } else if (child > 0) {
    int status;
    waitpid(child, &status, 0);
    return WEXITSTATUS(status);
  }

  return -1;
}

void xargs(int argc , char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: xargs [filename]" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string command_to_do;

  while(getline(std::cin, command_to_do)) {}

  for (int i = 1; i < argc; i++) {
    command_to_do.append(std::string(argv[i]));
    command_to_do.push_back(' ');
  }
  do_command(command_to_do);
}

int main(int argc , char *argv[]) {
  xargs(argc, argv);
}