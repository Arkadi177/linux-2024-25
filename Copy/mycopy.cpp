#include "../Utils/Iterator/Directory.h"
#include "../Utils/Iterator/ArgumentParser.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

void copy(std::string src, std::string dest) {

  struct stat st1;
  struct stat st2;

  if(stat(src.c_str(), &st1) == 0 && stat(dest.c_str(), &st2) == 0) {
    perror("Couldn't stat: ");
  }
  if(S_ISREG(st1.st_mode)) {
    if(S_ISREG(st2.st_mode)) {
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
  perror("Couldn't open file: ");
}

std::string name(std::string path) {
  return path.substr(path.find_last_of("/") + 1);
}

void copy_recursively(const std::string& src,const std::string& dest) {
  struct stat stat_buf;
  if (stat(src.c_str(), &stat_buf) == -1) {
    perror("stat");
    return;
  }

  if (S_ISDIR(stat_buf.st_mode)) {
    if (mkdir(dest.c_str(), 0777) == -1) {
      perror("mkdir");
      return;
    }

    Directory directory(const_cast<std::string&>(src));
    for (auto it = directory.begin(); it != directory.end(); ++it) {
      if (it.get_d_type() == DT_REG) {
        copy(*it, dest + "/" + name(*it));
      } else if (it.get_d_type() == DT_DIR) {
        copy_recursively(*it, dest + "/" + name(*it));
      }
    }
  } else if (S_ISREG(stat_buf.st_mode)) {
    copy(src, dest);
  } else {
    perror("No such file or directory");
  }
}

int main(int argc , char ** argv) {

  return 0;
}