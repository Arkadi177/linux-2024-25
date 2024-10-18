#include "ArgumentParser.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
int main() {
    std::vector<std::string> args;
    std::string input_line;
    std::getline(std::cin, input_line);
    std::istringstream iss(input_line);
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    int argc = args.size();
    char** argv = new char*[argc + 1];

    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[args[i].size() + 1];
        std::strcpy(argv[i], args[i].c_str());
    } // esi zut inputnem vekalum shata ankap gitem
    argv[argc] = nullptr;
    ArgumentParser parser(argc, argv , "ab:c:");
    for (const auto& arg : parser) {
        std::cout << arg->key << " " << arg->value.value_or("")<< "\n";
    }
    return 0;
}
