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
    Argument(std::optional<std::string> key, std::optional<std::string> value);

    bool is_flag() const noexcept;
    bool is_option_with_value() const noexcept;
    bool is_program_argument() const noexcept;
};

class ArgumentParser {
    int argc = 0;
    char** argv = nullptr;
    Argument* arg = nullptr;
    std::string m_optstring;

public:
    ArgumentParser(int argc, char** argv, std::string optstring);
    ~ArgumentParser();

    class Iterator {
        Argument* current_argument_ = nullptr;

    public:
        Iterator(Argument* current);
        void operator++();
        bool operator!=(const Iterator& other);
        Argument operator*();
    };

    void parse();
    Iterator begin() const;
    Iterator end() const;
};
