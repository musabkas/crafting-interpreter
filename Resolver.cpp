#include "Resolver.hpp"
#include "Loxi.hpp"

Resolver::Resolver(Interpreter* interpreter){
    this->interpreter = interpreter;
    scopes = {};
}

void Resolver::visitBlock(Block* stmt){
    beginScope();
    resolve(stmt->statements);
    endScope();
}

void Resolver::visitExpression(Expression* stmt) {
    resolve(stmt->expression);
}

void Resolver::visitVar(Var* stmt){
    declare(*stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    define(*stmt->name);
}

void Resolver::visitWhile(While* stmt){
    resolve(stmt->condition);
    resolve(stmt->body);
}

void Resolver::visitVariable(Variable* expr){
    if (!scopes.empty() && scopes.back().count(expr->name->lexeme) && scopes.back()[expr->name->lexeme] == false) {
        Loxi::error(*expr->name, "Can't read local variable in its own intializer.");
    }
    resolveLocal(expr, *expr->name);
}

void Resolver::visitAssign(Assign* expr) {
    resolve(expr->value);
    resolveLocal(expr, *expr->name);
}

void Resolver::visitBinary(Binary* expr) {
    resolve(expr->left);
    resolve(expr->right);
}

void Resolver::visitCall(Call* expr){
    resolve(expr->callee);

    for (auto& argument : expr->arguments){
        resolve(argument);
    }
}

void Resolver::visitGrouping(Grouping* expr){
    resolve(expr->expression);
}

void Resolver::visitLiteral(Literal* expr){
    return;
}

void Resolver::visitLogical(Logical* expr){
    resolve(expr->left);
    resolve(expr->right);
}

void Resolver::visitUnary(Unary* expr){
    resolve(expr->right);
}

void Resolver::visitFunction(Function* stmt) {
    declare(*stmt->name);
    define(*stmt->name);

    resolveFunction(stmt);
}

void Resolver::visitIf(If* stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) resolve(stmt->elseBranch);
}

void Resolver::visitPrint(Print* stmt){
    resolve(stmt->expression);
}

void Resolver::visitReturn(Return* stmt){
    if (stmt->value != nullptr){
        resolve(stmt->value);
    }
}


void Resolver::resolve(std::vector<std::unique_ptr<Stmt>>& statements){
    for (auto& statement : statements){
        resolve(statement);
    }
}

void Resolver::resolve(std::unique_ptr<Stmt>& stmt){
    stmt->acceptResolver(this);
}

void Resolver::resolve(std::unique_ptr<Expr>& expr){
    expr->acceptResolver(this);
}

void Resolver::resolveFunction(Function* function) {
    beginScope();
    for (auto& param : function->params){
        declare(*param);
        define(*param);
    }
    resolve(function->body);
    endScope();
}

void Resolver::beginScope(){
    scopes.push_back({});
}

void Resolver::endScope(){
    scopes.pop_back();
}

void Resolver::declare(Token name){
    if (scopes.empty()) return;

    scopes.back()[name.lexeme] = false;
}

void Resolver::define(Token name){
    if (scopes.empty()) return;
    scopes.back()[name.lexeme] = true;
}

void Resolver::resolveLocal(Expr* expr, Token name){
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].count(name.lexeme)) {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}