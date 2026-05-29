#include "TokenType.hpp"
#include <string>

class Token{
    TokenType type;
    std::string lexeme;
    std::variant<double, std::string> literal;
    int line;

public:
    Token(TokenType type, std::string lexeme, std::variant<double, std::string> literal, int line);
    std::string to_string();
};