#pragma once
#include "Interpreter.hpp"

enum FunctionType{
    NONE,
    FUNCTION,
    METHOD
};

class Resolver{
    Interpreter* interpreter;
    std::vector<std::unordered_map<std::string, bool>> scopes;
    FunctionType currentFunction;

    void resolve(std::unique_ptr<Stmt>& stmt);
    void resolve(std::unique_ptr<Expr>& expr);
    
    void resolveLocal(Expr* expr, Token name);
    void resolveFunction(Function* function, FunctionType type);
    
    void beginScope();
    void endScope();
    
    void declare(Token name);
    void define(Token name);

public:
    Resolver(Interpreter* interpreter);
    void resolve(std::vector<std::unique_ptr<Stmt>>& statements);
    
    void visitAssign(Assign* expr);
    void visitLogical(Logical* expr);
    void visitSet(Set* expr);
    void visitBinary(Binary* expr);
    void visitGrouping(Grouping* expr);
    void visitLiteral(Literal* expr);
    void visitUnary(Unary* expr);
    void visitCall(Call* expr);
    void visitGet(Get* expr);
    void visitVariable(Variable* expr);
    
    void visitExpression(Expression* stmt);
    void visitIf(If* stmt);
    void visitWhile(While* stmt);
    void visitVar(Var* stmt);
    void visitFunction(Function* stmt);
    void visitClass(Class* stmt);
    void visitReturn(Return* stmt);
    void visitPrint(Print* stmt);
    void visitBlock(Block* stmt);
};