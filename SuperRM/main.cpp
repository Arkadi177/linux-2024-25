#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "../Utils/Iterator/Directory.h"
#include "../Utils/Iterator/ArgumentParser.h"
#include "dirent.h"
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <stdexcept>

void delete_empty_dir(const std::string& path) {

  DIR* dir = opendir(path.c_str());
  if (!dir) {
    perror(("Failed to open directory: " + path).c_str());
  }
  dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string name = entry->d_name;
    if (name == "." || name == "..") {
      continue;
    }
    std::string full_path = path + "/" + name;
    if (entry->d_type == DT_DIR) {
      delete_empty_dir(full_path);
    }
  }
  closedir(dir);
  if (rmdir(path.c_str()) != 0) {
    perror(("Failed to delete directory: " + path).c_str());
  }

}

void super_rm(std::optional<std::string> path , bool verbose, bool recursive) {

  Directory dir(path.value());
  if(recursive == false) {
    DIR* dir_file = opendir(path.value().c_str());
    dirent* entry = readdir(dir_file);
    if(entry->d_type == DT_REG) {
      int fd = open(((*dir.begin())).c_str(), O_WRONLY);
      if(fd == -1) {
        perror("open");
      }
      write(fd , "\0" , 2);
      close(fd);
      unlink((*dir.begin()).c_str());
    }else {
      perror(("Not a File: " + path.value()).c_str());
    }
    closedir(dir_file);
    return;
  }
  for (auto it = dir.begin(); it != dir.end(); ++it) {
    if(verbose == true) {
      std::cout << *it << std::endl;
    }
    if(it.get_d_type() != DT_DIR) {
      int fd = open((*it).c_str(), O_WRONLY);
      if(fd == -1) {
          perror("open");
      }
      write(fd , "\0" , 2);
      close(fd);
      unlink((*it).c_str());
    }
  }
  delete_empty_dir(path.value());

}

int main(int argc , char ** argv) {

  ArgumentParser args(argc , argv , "r::v::");
  bool verbose = false;
  bool recursive = false;
  std::optional<std::string> path;
  for(auto it = args.begin(); it != args.end(); ++it) {
    if((*it).m_key == "v") {
      verbose = true;
      path = (*it).m_value;
    }else if((*it).m_key == "r") {
      recursive = true;
    }
  }
  super_rm(path , verbose , recursive);

}