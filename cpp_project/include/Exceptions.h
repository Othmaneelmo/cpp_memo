#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class OutOfRange : public std::out_of_range {
public:
    OutOfRange(const std::string& msg) : std::out_of_range(msg) {}
};

class NoMoreCards : public std::runtime_error {
public:
    NoMoreCards(const std::string& msg) : std::runtime_error(msg) {}
};

#endif
