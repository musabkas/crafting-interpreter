#pragma once
#include "Expr.hpp"
#include "Stmt.hpp"
#include <vector>

class Interpreter{
    LoxObject evaluate(Expr* expression);
    void execute(Stmt* stmt);
    bool isTruthy(LoxObject object);
    bool isEqual(LoxObject a, LoxObject b);

    void checkNumberOperand(Token op, LoxObject operand);
    void checkNumberOperands(Token op, LoxObject left, LoxObject right);

    std::string stringify(LoxObject object);

public:
    Interpreter() = default;
    LoxObject visitBinary(Binary* binary);
    LoxObject visitGrouping(Grouping* grouping);
    LoxObject visitLiteral(Literal* literal);
    LoxObject visitUnary(Unary* unary);

    void visitExpression(Expression* stmt);
    void visitPrint(Print* stmt);

    void interpret(std::vector<std::unique_ptr<Stmt>> statements);
};