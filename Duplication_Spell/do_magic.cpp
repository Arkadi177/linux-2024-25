#include <iostream>
#include <fcntl.h>
#include <unistd.h>

void do_magic() {
  int fd = open("../../Duplication_Spell/new_pts.txt" , O_RDONLY);
  dup2(fd , STDIN_FILENO);
  close(fd);
}

int main()
{

  do_magic();
  std::string s;
  std::cin >> s;
  std::cout << s;
  return 0;

}

