#include <iostream>
#include "Directory.h"
int main() {
    std::string path;
    std::cin >> path;
    Directory directory(path.c_str());
    for(Directory::recursive_directory_iterator it = directory.begin() ; it != directory.end(); ++it) {
        std::cout << *it << std::endl;
    }
}