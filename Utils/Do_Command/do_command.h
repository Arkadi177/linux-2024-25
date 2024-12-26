#ifndef DO_COMMAND_H
#define DO_COMMAND_H

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

#endif // DO_COMMAND_H
