#include "../Utils/Iterator/Directory.h"
#include "../Utils/Iterator/ArgumentParser.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

void copy(std::string src, std::string dest) {
  DIR *dir = opendir(src.c_str());
  DIR *dir2 = opendir(dest.c_str());
  dirent *entry = readdir(dir);
  dirent *entry2 = readdir(dir2);
  if(entry->d_type == DT_REG) {
    if(entry2->d_type != DT_REG) {
      perror(("Not a file: " + dest).c_str());
      return;
    }
    int fd = open(src.c_str(), O_RDONLY);
    int fd_dest = open(dest.c_str(), O_WRONLY | O_CREAT);
    char buffer[4096];
    while(read(fd, &buffer, 4096) > 0) {
      write(fd_dest, buffer, 4096);
    }
  }
  else {

  }
}

int main(int argc , char ** argv) {
  ArgumentParser args(argc , argv , "rnf");
  return 0;
}