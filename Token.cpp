#include "Token.hpp"
#include <iostream>

Token::Token(TokenType type, std::string lexeme, std::variant<double, std::string> literal, int line){
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}
std::string Token::to_string(){
    std::string r = std::to_string(type) + " " + lexeme + " ";
    if (std::holds_alternative<double>(literal))
        r += std::to_string((std::get<double>(literal)));
    else if (std::holds_alternative<std::string>(literal))
        r += std::get<std::string>(literal);
    return r;
}