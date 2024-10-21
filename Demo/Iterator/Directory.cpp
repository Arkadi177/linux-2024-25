#include <iostream>
#include "Directory.h"

int main() {
    std::string path;
    std::cin >> path;
    Directory directory(path.c_str());
    for(auto it = directory.begin() ; it != directory.end(); ++it) {
        std::cout << *it << std::endl;
    }
}