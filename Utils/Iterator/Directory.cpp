#include "Directory.h"

Directory::RecursiveDirectoryIterator::RecursiveDirectoryIterator(
    const std::string& file_name, dirent* entry, DIR* dir)
    : m_file_name(" "), m_dir(dir), m_entry(entry) {
    if (!file_name.empty()) {
        m_stack.push({file_name, 0});
    }
}

Directory::RecursiveDirectoryIterator& Directory::RecursiveDirectoryIterator::operator=(
    const Directory::RecursiveDirectoryIterator& other) {
    if (this == &other) {
        return *this;
    }
    m_dir = other.m_dir;
    m_file_name = other.m_file_name;
    m_entry = other.m_entry;
    return *this;
}

void Directory::RecursiveDirectoryIterator::operator++() {
    if (m_entry != nullptr && m_entry->d_type == DT_DIR &&
        std::string_view(m_entry->d_name) != "." && std::string_view(m_entry->d_name) != "..") {
        m_stack.push({m_stack.top().m_basename + "/" + m_entry->d_name, telldir(m_dir)});
        m_dir = opendir(m_stack.top().m_basename.c_str());
    }
    m_entry = readdir(m_dir);
    if (m_entry == nullptr && m_stack.size() > 1) {
        off_t off = m_stack.top().m_offset;
        m_stack.pop();
        m_dir = opendir(m_stack.top().m_basename.c_str());
        seekdir(m_dir, off);
        m_entry = readdir(m_dir);
    }
}

bool Directory::RecursiveDirectoryIterator::operator==(const Directory::RecursiveDirectoryIterator& other) const {
    return other.m_entry == m_entry;
}

bool Directory::RecursiveDirectoryIterator::operator!=(const Directory::RecursiveDirectoryIterator& other) const {
    return !(*this == other);
}

unsigned char Directory::RecursiveDirectoryIterator::get_d_type() {
    return m_entry->d_type;
}

std::string Directory::RecursiveDirectoryIterator::operator*() {
    m_file_name = m_stack.top().m_basename + "/" + m_entry->d_name;
    return m_file_name;
}

Directory::Directory(std::string& path) : path(path) {
    dir = opendir(path.c_str());
}

Directory::RecursiveDirectoryIterator Directory::begin() const {
    dirent* entry = readdir(dir);
    return {path, entry, dir};
}

Directory::RecursiveDirectoryIterator Directory::end() const {
    return {"", nullptr, dir};
}

Directory::~Directory() {
    closedir(dir);
}
