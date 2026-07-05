#pragma once
#include <vector>
#include "Token.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

class Parser {
private:
    int current;
    std::vector<std::unique_ptr<Token>> tokens;

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> orExpr();
    std::unique_ptr<Expr> andExpr();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclaration();
    std::vector<std::unique_ptr<Stmt>> block();
    
    bool match(std::vector<TokenType> types);

    bool check(TokenType type);
    Token advance();

    bool isAtEnd();
    Token peek();
    Token previous();

    class ParseError : public std::runtime_error {
        public:
        ParseError(const std::string& message) : std::runtime_error(message) {}
    };
    
    Token consume(TokenType type, std::string message);
    ParseError error(Token token, std::string message);
    void synchronize();

public:
    Parser(std::vector<std::unique_ptr<Token>> tokens);
    std::vector<std::unique_ptr<Stmt>> parse();
};