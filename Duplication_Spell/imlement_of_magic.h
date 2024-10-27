#pragma once
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
void do_magic() {
  int fd = open("../../Duplication_Spell/new_pts.txt" , O_RDONLY);
  dup2(fd , STDIN_FILENO);
  close(fd);
}