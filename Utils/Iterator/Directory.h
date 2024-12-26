#pragma once
#include <dirent.h>
#include <iostream>
#include <stack>

struct DirectoryDFSNodeData {
  std::string m_basename;
  off_t m_offset;
};

class Directory {

  std::string path;
  DIR* dir;

public:

  class RecursiveDirectoryIterator {

    std::string m_file_name;
    dirent* m_entry;
    DIR* m_dir;
    std::stack<DirectoryDFSNodeData> m_stack;

  public:
    RecursiveDirectoryIterator(const std::string& file_name, dirent* entry, DIR* dir);
    RecursiveDirectoryIterator& operator=(const RecursiveDirectoryIterator& other);
    void operator++();
    bool operator==(const RecursiveDirectoryIterator& other) const;
    bool operator!=(const RecursiveDirectoryIterator& other) const;
    unsigned char get_d_type();
    std::string operator*();
  };

  explicit Directory(std::string& path);
  RecursiveDirectoryIterator begin() const;
  RecursiveDirectoryIterator end() const;
  ~Directory();
};
