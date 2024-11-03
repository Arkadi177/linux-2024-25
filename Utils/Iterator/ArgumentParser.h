#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <optional>
#include <getopt.h>

struct Argument {
    std::optional<std::string> m_key;
    std::optional<std::string> m_value;

    Argument() = default;

    Argument(const Argument&) = default;
    Argument(Argument&&) noexcept = default;
    Argument& operator=(const Argument&) = default;
    Argument& operator=(Argument&&) noexcept = default;

    Argument(std::optional<std::string> key, std::optional<std::string> value)
        : m_key(std::move(key)), m_value(std::move(value)) {}

    bool is_flag() const noexcept {
        return m_key.has_value() && !m_value.has_value();
    }

    bool is_option_with_value() const noexcept {
        return m_key.has_value() && m_value.has_value();
    }

    bool is_program_argument() const noexcept {
        return !m_key.has_value() && m_value.has_value();
    }
};


class ArgumentParser {

    int argc = 0;
    char **argv = nullptr;
    Argument *arg = nullptr;
    std::string m_optstring;

public:

    ArgumentParser(int argc, char** argv , std::string optstring) : argc(argc), argv(argv) , m_optstring(std::move(optstring)) , arg(new Argument[argc]){
        parse();
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

        Argument operator*() {
            return *current_argument_;
        }

    };

    void parse() {
        optind = 1;
        opterr = 0;
        optopt = 0;
        int index = 0;
        int opt = 0;
        while ((opt = getopt(argc, argv, m_optstring.c_str())) != -1) {
            if(opt != '?') {
                std::string opt_str = std::string(1 , opt);
                arg[index].m_key = opt_str;
                if(optarg) {
                    arg[index].m_value = optarg;
                }
            }else {
                throw std::invalid_argument("Invalid character '?' encountered while parsing arguments.");
            }
            index++;
        }
        for(int i = optind; i < argc; i++) {
            std::string value = std::string(argv[i]);
            arg[index].m_value = value;
            index++;
        }
        argc = index; // to get the end of array
    }

    Iterator begin() const{
        return {arg};
    }

    Iterator end() const{
        return {arg + argc};
    }

};