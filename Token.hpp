#pragma once
#include "TokenType.hpp"
#include <string>

class Token{
    TokenType type;
    std::string lexeme;
    std::variant<double, std::string, void *> literal;
    int line;

public:
    Token(TokenType type, std::string lexeme, std::variant<double, std::string, void *> literal, int line);
    std::string to_string();

    friend std::ostream& operator<<(std::ostream& os, Token& token);
};