#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>

void do_command(const std::string& command_to_do) { // find it in man exec execl("/bin/sh", "sh", "-c", command, (char *) NULL);
  auto child = fork();
  if (child == 0) {
    execl("/bin/sh" , "sh" , "-c" ,command_to_do.c_str() , nullptr);
    exit(EXIT_SUCCESS);
  }else {
    int status;
    waitpid(-1, &status, 0);
    exit(EXIT_SUCCESS);
  }
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