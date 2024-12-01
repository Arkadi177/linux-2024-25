#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{

  int fd1 = open("../../Duplication_Spell/exclusive_file.log", O_WRONLY);
  if(fd1 == -1) {
    std::cerr << "File can be opened: " << strerror(errno) << std::endl;
  }

  int fd2 = dup(fd1);
  if(fd2 == -1) {
    std::cerr << "File can be duplicated " << strerror(errno) << std::endl;
  }

  std::string_view first_line = "First line:\n";
  std::string_view second_line = "Second line: ";

  write(fd1, first_line.data(), first_line.size());
  if(fd1 == -1) {
    std::cerr << "File can be written: " << strerror(errno) << std::endl;
  }

  write(fd2, second_line.data(), second_line.size());
  if(fd1 == -1) {
    std::cerr << "File can be written " << strerror(errno) << std::endl;
  }

  close(fd1);
  close(fd2);
  return 0;

}