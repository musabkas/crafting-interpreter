#pragma once
#include <vector>
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Environment.hpp"
#include "LoxFunction.hpp"

class Interpreter{
    std::unique_ptr<Environment> environment;
    
    LoxObject evaluate(Expr* expression);
    void execute(Stmt* stmt);
    bool isTruthy(LoxObject object);
    bool isEqual(LoxObject a, LoxObject b);
    
    void checkNumberOperand(Token op, LoxObject operand);
    void checkNumberOperands(Token op, LoxObject left, LoxObject right);
    
    std::string stringify(LoxObject object);
    
    public:
    Interpreter();
    Environment* globals;
    LoxObject visitAssign(Assign* assign);
    LoxObject visitLogical(Logical* logical);
    LoxObject visitBinary(Binary* binary);
    LoxObject visitGrouping(Grouping* grouping);
    LoxObject visitLiteral(Literal* literal);
    LoxObject visitUnary(Unary* unary);
    LoxObject visitCall(Call* call);
    LoxObject visitVariable(Variable* var);
    
    void visitExpression(Expression* stmt);
    void visitIf(If* stmt);
    void visitWhile(While* stmt);
    void visitVar(Var* stmt);
    void visitFunction(Function* stmt);
    void visitPrint(Print* stmt);
    void visitBlock(Block* stmt);
    
    void executeBlock(std::vector<std::unique_ptr<Stmt>>& statements, std::unique_ptr<Environment> environment);

    void interpret(std::vector<std::unique_ptr<Stmt>> statements);
};