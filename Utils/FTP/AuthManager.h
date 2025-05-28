#pragma once
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>

class AuthManager {
    std::unordered_map<std::string, std::string> credentials;
    std::string filename;

public:
    explicit AuthManager(const std::string& file = "config.txt") : filename(file) {
        std::ifstream in(filename);
        if (!in) {
            // Create empty file if it doesn't exist
            std::ofstream newFile(filename);
            if (!newFile) {
                std::cerr << "Error: Could not create config file: " << filename << "\n";
                return;
            }
        }

        std::string line;
        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string user, pass;
            if (std::getline(iss, user, '=') && std::getline(iss, pass)) {
                credentials[user] = pass;
            }
        }
    }

    // Returns true if valid login, otherwise appends new user and returns true
    bool validateOrAdd(const std::string& user, const std::string& pass) {
        auto it = credentials.find(user);
        if (it != credentials.end()) {
            return it->second == pass;
        }

        // Append new user to file
        std::ofstream out(filename, std::ios::app);
        if (out) {
            out << user << "=" << pass << "\n";
            credentials[user] = pass;
            std::cout << "New user added to config: " << user << "\n";
            return true;
        } else {
            std::cerr << "Error writing to config file.\n";
            return false;
        }
    }
};
