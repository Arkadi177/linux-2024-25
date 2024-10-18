#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <bits/getopt_core.h>
struct  Argument {
    char key;
    std::optional<std::string> value;
    Argument(char key, std::string value) : key(key) , value(std::move(value)) {}
    void set(char key, std::optional<std::string> value = std::nullopt) {
        this->key = key;
        this->value = std::move(value);
    }
    Argument() = default;
};
class ArgumentParser {
    int argc = 0;
    char **argv = nullptr;
    Argument *arg = nullptr;
    std::string optstring;
public:
    ArgumentParser(int argc, char** argv , std::string optstring) : argc(argc), argv(argv) , optstring(optstring) , arg(new Argument[2 * optstring.size()]){
        parse(argc , argv);
    }
    ~ArgumentParser() {
        delete[] arg;
    };
    class Iterator {
        Argument *current_argument_ = nullptr;
    public:
        Iterator(Argument* current) : current_argument_(current) {}
        void operator++() {
            current_argument_++;
        }
        bool operator!=(const Iterator& other) {
            return current_argument_ != other.current_argument_;
        }
        Argument* operator*() {
            return current_argument_;
        }
    };
    void parse(int argc, char** argv) {
        int opt = 0;
        int index = 0;
        while ((opt = getopt(argc, argv, optstring.c_str())) != -1) {
            if(opt != '?') {
                arg[index].key = static_cast<char>(opt);
                if(optarg) {
                    arg[index].value = optarg;
                }
            }else {
                std::cerr << "Shata vata!!!!!!" << std::endl;
            }
            index++;
        }
    }
    Iterator begin() {
        return {arg};
    }
    Iterator end() {
        int size = 0;
        for(const char i : optstring) {
            if(i != ':') {
                size++;
            }
        }
        return {arg + size};
    }
};