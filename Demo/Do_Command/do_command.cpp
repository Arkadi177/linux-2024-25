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
    wait(&status);
    if(WIFEXITED(status))
    {
      std::cout << "Process " << child << " exited with exit code " << WIFEXITED(status) <<"\n";
    }
    else if (WIFSIGNALED(status))
    {
      std::cout << "Process " << child << " was killed with signal " << WTERMSIG(status) << "\n";
    }
    std::cout << "Child process exited with status " << status << std::endl;
    exit(EXIT_SUCCESS);
  }
}

int main() {
  std::string command_to_do;
  getline(std::cin, command_to_do);
  do_command(command_to_do);
}