#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Token.hpp"

class Scanner{
private:
    std::string source;
    std::vector<std::unique_ptr<Token>> tokens;
    static std::map<std::string, TokenType> keywords;

    int start, current, line;

    bool isAtEnd();
    void scanToken();

    char advance();
    char peek();
    char peekNext();
    bool match(char expected);
    
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

    void addToken(TokenType type);
    void addToken(TokenType type, std::variant<double, std::string, void *> literal);

    void string();
    void number();
    void identifier();

public:
    Scanner(std::string source);
    std::vector<std::unique_ptr<Token>> scanTokens();
};