#pragma once
#include "Token.hpp"

class RuntimeError : public std::runtime_error {
public:
    Token token;
    RuntimeError(Token token, const std::string& message) : std::runtime_error(message), token(token) {}
};