#pragma once
#include <dirent.h>
#include <iostream>
#include <stack>

struct DirectoryDFSNodeData
{
  std::string m_basename;
  off_t m_offset;
};

class Directory {

  std::string path;
  DIR* dir;

 public:

  class RecursiveDirectoryIterator{

    std::string m_file_name;
    dirent* m_entry;
    DIR* m_dir;
    std::stack<DirectoryDFSNodeData> m_stack;

   public:

    RecursiveDirectoryIterator(const std::string& file_name, dirent* entry,
                                 DIR* dir)
        : m_file_name(" "), m_dir(dir), m_entry(entry) {
      if (!file_name.empty()) {
        m_stack.push({file_name, 0});
      }
    }

    RecursiveDirectoryIterator& operator=(
        const RecursiveDirectoryIterator& other) {
      if (this == &other) {
        return *this;
      }
      m_dir = other.m_dir;
      m_file_name = other.m_file_name;
      m_entry = other.m_entry;
      return *this;
    }

    void operator++() {
      if (m_entry != nullptr && m_entry->d_type == DT_DIR &&
          std::string_view(m_entry->d_name) != "." &&
          std::string_view(m_entry->d_name) != "..") {
          m_stack.push({m_stack.top().m_basename + "/" + m_entry->d_name, telldir(m_dir)});
          m_dir = opendir(m_stack.top().m_basename.c_str());
      }
      m_entry = readdir(m_dir);
      if (m_entry == nullptr && m_stack.size() > 1) {
          off_t off = m_stack.top().m_offset;
          m_stack.pop();
          m_dir = opendir(m_stack.top().m_basename.c_str());
          seekdir(m_dir , off);
          m_entry = readdir(m_dir);
      }
    }

    bool operator==(const RecursiveDirectoryIterator& other) const {
      return other.m_entry == m_entry;
    }

    bool operator!=(const RecursiveDirectoryIterator& other) const {
      return !(*this == other);
    }

    std::string operator*()  {
      m_file_name = m_stack.top().m_basename + "/" + m_entry->d_name;
      return m_file_name;
    }
  };

  explicit Directory(std::string& path) : path(path) {
    dir = opendir(path.c_str());
  }

  RecursiveDirectoryIterator begin() const {
    dirent* entry = readdir(dir);
    return {path, entry, dir};
  }

  RecursiveDirectoryIterator end() const {
    return {"", nullptr, dir};
  }

  ~Directory() {
    closedir(dir);
  }

};