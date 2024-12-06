#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "dr_watcher.h"
#include <dirent.h>
#include "Directory.h"

void test1() {
  auto fd = open("test1.txt", O_CREAT | O_RDWR | O_TRUNC , 0666);
  if (fd == -1) {
    std::cerr << "Failed to open watcher_test.txt" << std::strerror(errno) << std::endl;
    exit(1);
  }
  DirectoryWatcher watcher("test1.txt");
  watcher.watch();
}

void test2() {
  DirectoryWatcher watcher("testdir");
  auto dir_d = opendir("testdir");
  if (dir_d == nullptr) {
    std::cerr << "Failed to open directory" << std::strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string path = "testdir";

  watcher.watch();
}

int main() {
  test1();
  test2();
}
