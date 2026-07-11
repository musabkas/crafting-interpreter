#include "Parser.hpp"
#include "Loxi.hpp"
#include <iostream>

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

std::unique_ptr<Stmt> Parser::statement(){
    if (match({FOR})) return std::move(forStatement());
    if (match({IF})) return std::move(ifStatement());
    if (match({PRINT})) return std::move(printStatement());
    if (match({RETURN})) return std::move(returnStatement());
    if (match({WHILE})) return std::move(whileStatement());
    if (match({LEFT_BRACE})) return std::make_unique<Block> (block());
    return std::move(expressionStatement());
}

std::unique_ptr<Stmt> Parser::declaration(){
    try {
        if (match({CLASS})) return classDeclaration();
        if (match({FUN})) return function("function");
        if (match({VAR})) return varDeclaration();

        return statement();
    } catch (ParseError error) {
        synchronize();
    }
}

std::unique_ptr<Stmt> Parser::classDeclaration(){
    std::unique_ptr<Token> name = std::make_unique<Token>(consume(IDENTIFIER, "Expect class name."));
    consume(LEFT_BRACE, "Expect '{' before class body.");
    std::vector<std::unique_ptr<Function>> methods = {};
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        methods.emplace_back(function("method"));
    }

    consume(RIGHT_BRACE, "Expect '}' after class body.");
    return std::make_unique<Class>(std::move(name), std::move(methods));
}

std::unique_ptr<Function> Parser::function(std::string kind){
    std::unique_ptr<Token> name = std::make_unique<Token>(consume(IDENTIFIER, "Expect " + kind + " name."));
    consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<std::unique_ptr<Token>> parameters = {};
    if (!check(RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Can't have more than 255 parameters.");
            }
            parameters.emplace_back(std::make_unique<Token>(consume(IDENTIFIER, "Expect parameter name.")));
        } while (match({COMMA}));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");
    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::unique_ptr<Stmt>> body = block();
    return std::make_unique<Function>(std::move(name), std::move(parameters), std::move(body));
}

std::unique_ptr<Stmt> Parser::varDeclaration(){
    std::unique_ptr<Token> name = std::make_unique<Token> (consume(IDENTIFIER, "Expect variable name"));

    std::unique_ptr<Expr> initializer = nullptr;
    if (match({EQUAL})) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<Var>(std::move(name), std::move(initializer));
}

std::unique_ptr<Stmt> Parser::ifStatement(){
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if.");

    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match({ELSE})){
        elseBranch = statement();
    }

    return std::make_unique<If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::whileStatement(){
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after while.");

    std::unique_ptr<Stmt> body = statement();

    return std::make_unique<While>(std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::forStatement(){
    consume(LEFT_PAREN, "Expect '(' after 'for'.");
    std::unique_ptr<Stmt> initializer;
    if (match({SEMICOLON})) {
        initializer = nullptr;
    } else if (match({VAR})) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    std::unique_ptr<Expr> condition = nullptr;
    if (!check(SEMICOLON)) {
        condition = expression();
    }
    consume(SEMICOLON, "Expect ';' after loop condition.");

    std::unique_ptr<Expr> increment = nullptr;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");

    std::unique_ptr<Stmt> body = statement();

    if (increment != nullptr) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.emplace_back(std::move(body));
        statements.emplace_back(std::make_unique<Expression>(std::move(increment)));
        body = std::make_unique<Block>(std::move(statements));
    }

    if (condition == nullptr) condition = std::make_unique<Literal>(LoxObject(std::in_place_type<bool>, true));
    body = std::make_unique<While>(std::move(condition), std::move(body));

    if (initializer != nullptr) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.emplace_back(std::move(initializer));
        statements.emplace_back(std::move(body));
        body = std::make_unique<Block>(std::move(statements));
    }

    return std::move(body);
}

std::unique_ptr<Stmt> Parser::returnStatement(){
    std::unique_ptr<Token> keyword = std::make_unique<Token>(previous());
    std::unique_ptr<Expr> value = nullptr;
    if (!check(SEMICOLON)) {
        value = expression();
    }
    consume(SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<Return> (std::move(keyword), std::move(value));
}

std::unique_ptr<Stmt> Parser::printStatement(){
    std::unique_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return std::make_unique<Print> (std::move(value));
}

std::unique_ptr<Stmt> Parser::expressionStatement(){
    std::unique_ptr<Expr> expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<Expression> (std::move(expr));
}

std::vector<std::unique_ptr<Stmt>> Parser::block(){
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd()){
        statements.emplace_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return std::move(statements);
}

std::unique_ptr<Expr> Parser::expression(){
    return std::move(assignment());
}

std::unique_ptr<Expr> Parser::assignment(){
    std::unique_ptr<Expr> expr = std::move(orExpr());

    if (match({EQUAL})) {
        std::unique_ptr<Token> equals = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> value = std::move(assignment());

        if (auto var = dynamic_cast<Variable*>(expr.get())){
            std::unique_ptr<Token> name = std::make_unique<Token>(*(var->name));
            return std::make_unique<Assign>(std::move(name), std::move(value));
        }

        error(*equals, "Invalid assignment target.");
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::orExpr(){
    std::unique_ptr<Expr> expr = std::move(andExpr());

    while (match({OR})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(andExpr());
        expr = std::make_unique<Logical>(std::move(expr), std::move(op), std::move(right));
    }
    return std::move(expr);
}

std::unique_ptr<Expr> Parser::andExpr(){
    std::unique_ptr<Expr> expr = std::move(equality());

    while (match({AND})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(equality());
        expr = std::make_unique<Logical>(std::move(expr), std::move(op), std::move(right));
    }
    return std::move(expr);
}


std::unique_ptr<Expr> Parser::equality(){
    std::unique_ptr<Expr> expr = std::move(comparison());

    while (match({BANG_EQUAL, EQUAL_EQUAL})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous()); // using copy constructor to create duplicate of token so that it is not deleted when token unique_ptr is deleted from stack
        std::unique_ptr<Expr> right = std::move(comparison());
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::comparison(){
    std::unique_ptr<Expr> expr = std::move(term());
    
    while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(term());
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }
    
    return std::move(expr);
}

std::unique_ptr<Expr> Parser::term(){
    std::unique_ptr<Expr> expr = std::move(factor());
    
    while (match({MINUS, PLUS})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(factor());
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::factor(){
    std::unique_ptr<Expr> expr = std::move(unary());
    
    while (match({SLASH, STAR})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(unary());
        expr = std::make_unique<Binary>(std::move(expr), std::move(op), std::move(right));
    }

    return std::move(expr);
}

std::unique_ptr<Expr> Parser::unary(){
    if (match({BANG, MINUS})) {
        std::unique_ptr<Token> op = std::make_unique<Token>(previous());
        std::unique_ptr<Expr> right = std::move(unary());
        return std::make_unique<Unary>(std::move(op), std::move(right));
    }
    return call();
}

std::unique_ptr<Expr> Parser::call(){
    std::unique_ptr<Expr> expr = std::move(primary());

    while (true) {
        if (match({LEFT_PAREN})) {
            expr = finishCall(std::move(expr));
        } else {
            break;
        }
    }
    
    return std::move(expr);
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> callee){
    std::vector<std::unique_ptr<Expr>> arguments = {};
    if (!check(RIGHT_PAREN)){
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments.emplace_back(expression());
        } while (match({COMMA}));
    }

    std::unique_ptr<Token> paren = std::make_unique<Token>(consume(RIGHT_PAREN, "Expect ')' after arguments."));
    return std::make_unique<Call>(std::move(callee), std::move(paren), std::move(arguments));
}

std::unique_ptr<Expr> Parser::primary(){
    if (match({FALSE})) return std::make_unique<Literal>(false);
    if (match({TRUE})) return std::make_unique<Literal>(true);
    if (match({NIL})) return std::make_unique<Literal>(nullptr);

    if (match({NUMBER})) {
        return std::make_unique<Literal>(std::get<double>(previous().literal));
    }
    if (match({STRING})) {
        return std::make_unique<Literal>(std::get<std::string>(previous().literal));
    }
    if (match({IDENTIFIER})){
        return std::make_unique<Variable>(std::make_unique<Token> (previous()));
    }

    if (match({LEFT_PAREN})){
        std::unique_ptr<Expr> expr = std::move(expression());
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }
    throw error(peek(), "Expect expression.");
}

Token Parser::consume(TokenType type, std::string message){
    if (check(type)) return advance();
    throw error(peek(), message);
}

Parser::ParseError Parser::error(Token token, std::string message){
    Loxi::error(token, message);
    return ParseError(message);
}

void Parser::synchronize(){
    advance();

    while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;

        switch(peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
            default:
                advance();
        }
    }
}

std::vector<std::unique_ptr<Stmt>> Parser::parse(){
    std::vector<std::unique_ptr<Stmt>> statements = {};
    while (!isAtEnd()){
        statements.emplace_back(declaration());
    }
    return std::move(statements);
}