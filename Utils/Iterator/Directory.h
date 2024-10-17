#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <dirent.h>
char* vectorToCString(const std::vector<std::string>& vec, const char* separator = "/") {
    std::string resultStr;
    for (size_t i = 0; i < vec.size(); ++i) {
        resultStr += vec[i];
        if (i != vec.size() - 1) {
            resultStr += separator;
        }
    }
    char* result = new char[resultStr.size() + 1];
    for (size_t i = 0; i < resultStr.size(); ++i) {
        result[i] = resultStr[i];
    }
    result[resultStr.size()] = '\0';
    return result;
}
class Directory {
    std::string path;
    DIR *dir;
public:
    class recursive_directory_iterator {
        std::vector<std::string> path;
        std::string file_name;
        dirent *entry;
        DIR *dir;
        std::stack<long> positions;
    public:
        recursive_directory_iterator(const std::string& file_name , dirent* entry , DIR* dir) : file_name(file_name), dir(dir) , entry(entry){
            path.push_back(file_name);
        }
        recursive_directory_iterator& operator=(const recursive_directory_iterator& other) {
            dir = other.dir;
            file_name = other.file_name;
            entry = other.entry;
            return *this;
        }
        void operator++() {
            if(entry != nullptr && entry->d_type == DT_DIR && entry->d_name[0] != '.') {
                path.emplace_back(entry->d_name);
                positions.emplace(telldir(dir));
                dir = opendir(vectorToCString(path));
            }
            entry = readdir(dir);
            if(entry == nullptr && path.size() > 1) {
                path.pop_back();
                dir = opendir(vectorToCString(path));
                seekdir(dir , positions.top());
                positions.pop();
                entry = readdir(dir);
            }
        }
        bool operator==(const recursive_directory_iterator& other) const{
            if(other.entry == entry && path.size() == other.path.size()) {
                return true;
            }
            return false;
        }
        bool operator!=(const recursive_directory_iterator& other) const {
            return !(*this == other);
        }
        char* operator*()const {
            return entry->d_name;
        }
    };

    explicit Directory(const char *path) : path(path) {
        dir = opendir(path);
    }
    recursive_directory_iterator begin()const {
        dirent* entry = readdir(dir);
        return {path, entry, dir};
    }
    recursive_directory_iterator end()const {
        return {path, nullptr, dir};
    }
    ~Directory() {
        closedir(dir);
    }
};