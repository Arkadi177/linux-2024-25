#include "ArgumentParser.h"
#include <stdexcept>

Argument::Argument(std::optional<std::string> key, std::optional<std::string> value)
    : m_key(std::move(key)), m_value(std::move(value)) {}

bool Argument::is_flag() const noexcept {
    return m_key.has_value() && !m_value.has_value();
}

bool Argument::is_option_with_value() const noexcept {
    return m_key.has_value() && m_value.has_value();
}

bool Argument::is_program_argument() const noexcept {
    return !m_key.has_value() && m_value.has_value();
}

ArgumentParser::ArgumentParser(int argc, char** argv, std::string optstring)
    : argc(argc), argv(argv), m_optstring(std::move(optstring)), arg(new Argument[argc]) {
    parse();
}

ArgumentParser::~ArgumentParser() {
    delete[] arg;
}

ArgumentParser::Iterator::Iterator(Argument* current) : current_argument_(current) {}

void ArgumentParser::Iterator::operator++() {
    current_argument_++;
}

bool ArgumentParser::Iterator::operator!=(const Iterator& other) {
    return current_argument_ != other.current_argument_;
}

Argument ArgumentParser::Iterator::operator*() {
    return *current_argument_;
}

void ArgumentParser::parse() {
    optind = 1;
    opterr = 0;
    optopt = 0;
    int index = 0;
    int opt = 0;
    while ((opt = getopt(argc, argv, m_optstring.c_str())) != -1) {
        if(opt != '?') {
            std::string opt_str = std::string(1, opt);
            arg[index].m_key = opt_str;
            if(optarg) {
                arg[index].m_value = optarg;
            }
        } else {
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

ArgumentParser::Iterator ArgumentParser::begin() const {
    return {arg};
}

ArgumentParser::Iterator ArgumentParser::end() const {
    return {arg + argc};
}
