#include "Token.hpp"
#include "Expr.hpp"
#include <vector>

class Parser {
private:
    int current;
    std::vector<std::unique_ptr<Token>> tokens;

    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    bool match(std::vector<TokenType> types);

    bool check(TokenType type);
    Token advance();

    bool isAtEnd();
    Token peek();
    Token previous();

public:
    Parser(std::vector<std::unique_ptr<Token>> tokens);
    std::unique_ptr<Expr> expression();
};