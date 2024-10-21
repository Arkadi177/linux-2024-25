#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <dirent.h>

std::string vectorToString(const std::vector<std::string>& vec, const std::string& separator = "/") {
    std::string resultStr;
    for (size_t i = 0; i < vec.size(); ++i) {
        resultStr += vec[i];
        if (i != vec.size() - 1) {
            resultStr += separator;
        }
    }
    return resultStr;
}

class Directory {
    std::string path;
    DIR *dir;
public:
    class Recursive_directory_iterator {
        std::vector<std::string> path;
        std::string file_name;
        dirent *entry;
        DIR *dir;
        std::stack<long> positions;
    public:
        Recursive_directory_iterator(const std::string& file_name , dirent* entry , DIR* dir) : file_name(file_name), dir(dir) , entry(entry){
            if(!file_name.empty()) {
                path.push_back(file_name);
            }
        }
        Recursive_directory_iterator& operator=(const Recursive_directory_iterator& other) {
            if (this == &other) {
                return *this;
            }
            dir = other.dir;
            file_name = other.file_name;
            entry = other.entry;
            return *this;
        }
        void operator++() {
            if(entry != nullptr && entry->d_type == DT_DIR && std::string(entry->d_name) !=  "." && std::string(entry->d_name) != "..") {
                path.emplace_back(entry->d_name);
                positions.emplace(telldir(dir));
                dir = opendir(vectorToString(path).c_str());
            }
            entry = readdir(dir);
            if(entry == nullptr && path.size() > 1) {
                path.pop_back();
                dir = opendir(vectorToString(path).c_str());
                seekdir(dir , positions.top());
                positions.pop();
                entry = readdir(dir);
            }
        }
        bool operator==(const Recursive_directory_iterator& other) const{
            if(other.entry == entry) {
                return true;
            }
            return false;
        }
        bool operator!=(const Recursive_directory_iterator& other) const {
            return !(*this == other);
        }
        char* operator*()const {
            return entry->d_name;
        }
    };

    explicit Directory(const char *path) : path(path) {
        dir = opendir(path);
    }
    Recursive_directory_iterator begin() const {
        dirent* entry = readdir(dir);
        return {path, entry, dir};
    }
    Recursive_directory_iterator end() const {
        return {"", nullptr, dir};
    }
    ~Directory() {
        closedir(dir);
    }
};