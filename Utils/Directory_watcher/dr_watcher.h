#ifndef DR_WATCHER_H
#define DR_WATCHER_H

#include <algorithm>
#include <sys/inotify.h>
#include <iostream>
#include <sys/stat.h>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

enum Status {
  IS_FILE,
  IS_DIRECTORY
};

Status check_status(const std::string& pathname) {
  struct stat path_stat{};
  if(stat(pathname.c_str(), &path_stat) == 0) {
    if(S_ISDIR(path_stat.st_mode)) {
      return IS_DIRECTORY;
    }else if(S_ISREG(path_stat.st_mode)) {
      return IS_FILE;
    }
  }else {
    std::cerr << "Pathname " << pathname << "didn't exist: " << std::strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
}

std::string get_name(const std::string& pathname) {
  std::string filename = " ";
  for(auto i = pathname.end() - 1; ; i--) {
    if(*i == '/') {
      break;
    }
    filename += *i;
    if(i == pathname.begin()) {
      break;
    }
  }
  std::reverse(filename.begin(), filename.end());
  return filename;
}

constexpr unsigned int MAX_EVENTS_COUNT = 128;
constexpr unsigned int EVENT_SIZE = sizeof(struct inotify_event); // struct inotify doesn't include the size of event and
constexpr unsigned int MAX_SIZE_FILENAME = 64;
constexpr unsigned int BUF_SIZE = ( EVENT_SIZE + MAX_SIZE_FILENAME ); // cause the depends on filename size

class DirectoryWatcher {

  int watch_fd;
  int fd;
  Status status;
  std::string name;

public:

  explicit DirectoryWatcher(const std::string &pathname)
    : watch_fd(-1) , fd(-1) , status(check_status(pathname)) , name(get_name(pathname)){
    fd = inotify_init1(0);
    if(fd == -1) {
      std::cerr << "Inotify_init failed: " << pathname << std::strerror(errno) << std::endl;
      exit(EXIT_FAILURE);
    }
    if(status == IS_FILE) {
      watch_fd = inotify_add_watch(fd , pathname.c_str(), IN_ACCESS | IN_MODIFY | IN_ATTRIB |
        IN_DELETE_SELF);
    }else {
      watch_fd = inotify_add_watch(fd , pathname.c_str(), IN_CREATE | IN_MODIFY | IN_DELETE);
    }
    if(watch_fd == -1) {
      std::cerr << "Inotify_add_watch failed: " << pathname << std::strerror(errno)  << std::endl;
    }
  }

  void watch() const{
    long len = 1;
    char buffer[BUF_SIZE];
    while((len = read(fd, buffer, sizeof(buffer)))) {
      if(len == -1) {
        std::cerr << "Read failed: " << name << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
      }
      if(len == 0) {
        break;
      }
      for(unsigned int i = 0; i < len; ) {
        auto event = reinterpret_cast<const struct inotify_event*>(&buffer[i]);
        if(status == IS_FILE) {
          if(event->mask & IN_MODIFY) {
            std::cout << "File Modified: "  << name << std::endl;
          }
          if(event->mask & IN_DELETE_SELF) {
            std::cout << "File Attributes changed: " << name <<std::endl;
            return;
          }
          if(event->mask & IN_ATTRIB) {
            std::cout << "File Deleted: "  << name << std::endl;
          }
          if(event->mask & IN_ACCESS) {
            std::cout << "File Accessed: " << name << std::endl;
          }
          if (event->mask & IN_IGNORED) {
            std::cout << "Watch target deleted or unlinked: " << name << std::endl;
            return;
          }
        }else {
          if(event->mask & IN_MODIFY) {
            std::cout << "Modify: "  << name << std::endl;
          }
          if(event->mask & IN_DELETE) {
            std::cout << "Deleted: "  << name << std::endl;
            return;
          }
          if(event->mask & IN_CREATE) {
            std::cout << "Created: "  << name << std::endl;
          }
        }
        i += event->len + EVENT_SIZE;
      }
    }
  }

  ~DirectoryWatcher() {
    inotify_rm_watch(fd, watch_fd);
    close(watch_fd);
    close(fd);
  }

};

#endif //DR_WATCHER_H