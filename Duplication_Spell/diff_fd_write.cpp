#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main()
{

  int fd1 = open("../../Duplication_Spell/exclusive_file.log", O_WRONLY);
  int fd2 = dup(fd1);
  const char* first_line = "First line:\n";
  const char* second_line = "Second line: ";
  write(fd1, first_line, strlen(first_line));
  write(fd2, second_line, strlen(second_line));
  close(fd1);
  close(fd2);
  return 0;

}