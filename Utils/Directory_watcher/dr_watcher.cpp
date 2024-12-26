#include "dr_watcher.h"

File_Type check_status(const std::string& pathname) {
  struct stat path_stat{};
  if(stat(pathname.c_str(), &path_stat) == 0) {
    if(S_ISDIR(path_stat.st_mode)) {
      return IS_DIRECTORY;
    } else if(S_ISREG(path_stat.st_mode)) {
      return IS_FILE;
    }
  } else {
    throw std::runtime_error("File not found");
  }
}

DirectoryWatcher::DirectoryWatcher(const std::string &pathname)
    : watch_fd(-1), fd(-1), status(check_status(pathname)), name(pathname) {
  fd = inotify_init1(0);
  if(fd == -1) {
    throw std::runtime_error("inotify_init1() failed");
  }
  if(status == IS_FILE) {
    watch_fd = inotify_add_watch(fd, pathname.c_str(), IN_ACCESS | IN_MODIFY | IN_ATTRIB | IN_DELETE_SELF);
  } else {
    watch_fd = inotify_add_watch(fd, pathname.c_str(), IN_CREATE | IN_MODIFY | IN_DELETE);
  }
  if(watch_fd == -1) {
    throw std::runtime_error("inotify_add_watch() failed");
  }
}

void DirectoryWatcher::watch() const {
  long len = 1;
  char buffer[BUF_SIZE];
  while((len = read(fd, buffer, sizeof(buffer)))) {
    if(len == -1) {
      throw std::runtime_error("read() failed");
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
          std::cout << "File Attributes changed: " << name << std::endl;
        }
        if(event->mask & IN_ATTRIB) {
          std::cout << "File Deleted: "  << name << std::endl;
        }
        if(event->mask & IN_ACCESS) {
          std::cout << "File Accessed: " << name << std::endl;
        }
        if(event->mask & IN_IGNORED) {
          std::cout << "Watch target deleted or unlinked: " << name << std::endl;
        }
      } else {
        if(event->mask & IN_MODIFY) {
          std::cout << "Modify: "  << name << std::endl;
        }
        if(event->mask & IN_DELETE) {
          std::cout << "Deleted: "  << name << std::endl;
        }
        if(event->mask & IN_CREATE) {
          std::cout << "Created: "  << name << std::endl;
        }
      }
      i += event->len + EVENT_SIZE;
    }
  }
}

DirectoryWatcher::~DirectoryWatcher() {
  inotify_rm_watch(fd, watch_fd);
  close(watch_fd);
  close(fd);
}
