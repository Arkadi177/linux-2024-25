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
#include <filesystem>

enum File_Type {
  IS_FILE,
  IS_DIRECTORY
};

File_Type check_status(const std::string& pathname);

constexpr unsigned int MAX_EVENTS_COUNT = 128;
constexpr unsigned int EVENT_SIZE = sizeof(struct inotify_event); // struct inotify doesn't include the size of event and
constexpr unsigned int MAX_SIZE_FILENAME = 64;
constexpr unsigned int BUF_SIZE = ( EVENT_SIZE + MAX_SIZE_FILENAME ); // cause the depends on filename size

class DirectoryWatcher {
  int watch_fd;
  int fd;
  File_Type status;
  std::filesystem::path name;

public:
  explicit DirectoryWatcher(const std::string &pathname);
  void watch() const;
  ~DirectoryWatcher();
};

#endif // DR_WATCHER_H
