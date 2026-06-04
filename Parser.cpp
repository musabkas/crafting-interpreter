#include "Parser.hpp"

Parser::Parser(std::vector<std::unique_ptr<Token>> tokens){
    this->tokens = std::move(tokens);
    this->current = 0;
}

bool Parser::match(std::vector<TokenType> types){
    for (TokenType type : types){
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::check(TokenType type){
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance(){
    if (!isAtEnd()) current++;
    return previous();
}


bool Parser::isAtEnd(){
    return peek().type == END_OF_FILE;
}

Token Parser::peek(){
    return *tokens[current];
}

Token Parser::previous(){
    return *tokens[current - 1];
}

std::unique_ptr<Expr> Parser::expression(){
    return std::move(equality());
}

std::unique_ptr<Expr> Parser::equality(){
    std::unique_ptr<Expr> expr = std::move(comparison());

    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous()); // using copy constructor to create duplicate of token so that it is not deleted when token unique_ptr is deleted from stack
        std::unique_ptr<Expr> right = std::move(comparison());
        expr = std::make_unique<Binary>(expr, op, right);
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::comparison(){
    std::unique_ptr<Expr> expr = std::move(term());
    
    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(term());
        expr = std::make_unique<Binary>(expr, op, right);
    }
    
    return std::move(expr);
}

std::unique_ptr<Expr> Parser::term(){
    std::unique_ptr<Expr> expr = std::move(factor());
    
    while (match({MINUS, PLUS})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(factor());
        expr = std::make_unique<Binary>(expr, op, right);
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::factor(){
    std::unique_ptr<Expr> expr = std::move(unary());
    
    while (match({SLASH, STAR})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(unary());
        expr = std::make_unique<Binary>(expr, op, right);
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::unary(){
    if (match({BANG, MINUS})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(unary());
        return std::move(std::make_unique<Unary>(op, right));
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary(){
    if (match({FALSE})) return std::move(std::make_unique<Literal>(false));
    if (match({TRUE})) return std::move(std::make_unique<Literal>(true));
    if (match({NIL})) return std::move(std::make_unique<Literal>(nullptr));

    if (match({NUMBER, STRING})) {
        return std::move(std::make_unique<Literal>(previous().literal));
    }

    if (match({LEFT_PAREN})){
        std::unique_ptr<Expr> expr = std::move(expression());
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::move(std::make_unique<Grouping>(expr));
    }
}