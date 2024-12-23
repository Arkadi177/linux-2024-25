#ifndef DO_COMMAND_H
#define DO_COMMAND_H

#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>

int do_command(const std::string& command_to_do) { // find it in man exec execl("/bin/sh", "sh", "-c", command, (char *) NULL);
  auto child = fork();
  if (child == 0) {
    execl("/bin/sh" , "sh" , "-c" ,command_to_do.c_str() , nullptr);
    return -1;
  }else {
    int status;
    waitpid(-1, &status, 0);
    return WEXITSTATUS(status);
  }
}
#endif //DO_COMMAND_H
