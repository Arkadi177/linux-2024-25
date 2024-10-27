#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main()
{

  int fd1 = open("../../Duplication_Spell/exclusive_file.log", O_WRONLY);
  int fd2 = dup(fd1);
  std::string_view first_line = "First line:\n";
  std::string_view second_line = "Second line: ";
  write(fd1, first_line.data(), first_line.size());
  write(fd2, second_line.data(), second_line.size());
  close(fd1);
  close(fd2);
  return 0;

}