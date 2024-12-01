#pragma once
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void do_magic() {

  int fd = open("../../Duplication_Spell/new_pts.txt" , O_RDONLY);
  if(fd == -1) {
    std::cerr << "File can be opened: " << strerror(errno) << std::endl;
  }

  dup2(fd , STDIN_FILENO);
  if(fd == -1) {
    std::cerr << "File can be duplicated " << strerror(errno) << std::endl;
  }

  close(fd);

}