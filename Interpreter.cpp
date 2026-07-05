#include "Interpreter.hpp"
#include "RuntimeError.hpp"
#include "Loxi.hpp"
#include <iostream>

Interpreter::Interpreter(){
    environment = std::make_unique<Environment>();
}

LoxObject Interpreter::visitLiteral(Literal* literal){
    return literal->value;
}

LoxObject Interpreter::visitGrouping(Grouping* grouping){
    return evaluate(grouping->expression.get());
}

LoxObject Interpreter::evaluate(Expr* expression){
    return expression->acceptInterpreter(this);
}

LoxObject Interpreter::visitAssign(Assign* assign){
    LoxObject value = evaluate(assign->value.get());
    environment->assign(*(assign->name), value);
    return value;
}

LoxObject Interpreter::visitUnary(Unary* unary){
    LoxObject right = evaluate(unary->right.get());
    
    switch(unary->op->type) {
        case BANG:
        return LoxObject(std::in_place_type<bool>, !isTruthy(right));
        case MINUS:
        checkNumberOperand(*unary->op, right);
        return LoxObject(std::in_place_type<double>, -std::get<double>(right));
    }
    
    return LoxObject(std::in_place_type<void*>, nullptr);
}

LoxObject Interpreter::visitLogical(Logical* logical){
    LoxObject left = evaluate(logical->left.get());

    if (logical->op->type == OR) {
        if (isTruthy(left)) return left;
    } else {
        if (!isTruthy(left)) return left;
    }

    return evaluate(logical->right.get());
}

LoxObject Interpreter::visitBinary(Binary* binary){
    LoxObject left = evaluate(binary->left.get());
    LoxObject right = evaluate(binary->right.get());

    switch (binary->op->type) {
        case GREATER: 
            checkNumberOperands(*binary->op, left, right);
            return LoxObject(std::in_place_type<bool>, std::get<double>(left) > std::get<double>(right));
        case GREATER_EQUAL: 
            checkNumberOperands(*binary->op, left, right);
            return LoxObject(std::in_place_type<bool>, std::get<double>(left) >= std::get<double>(right));
        case LESS: 
            checkNumberOperands(*binary->op, left, right);   
            return LoxObject(std::in_place_type<bool>, std::get<double>(left) < std::get<double>(right));
        case LESS_EQUAL: 
            checkNumberOperands(*binary->op, left, right);   
            return LoxObject(std::in_place_type<bool>, std::get<double>(left) <= std::get<double>(right));

        case BANG_EQUAL: return LoxObject(std::in_place_type<bool>, !isEqual(left, right));
        case EQUAL_EQUAL: return LoxObject(std::in_place_type<bool>, isEqual(left, right));

        case MINUS: 
            checkNumberOperands(*binary->op, left, right);   
            return LoxObject(std::in_place_type<double>, std::get<double>(left) - std::get<double>(right));
        case SLASH: 
            checkNumberOperands(*binary->op, left, right);   
            return LoxObject(std::in_place_type<double>, std::get<double>(left) / std::get<double>(right));
        case STAR: 
            checkNumberOperands(*binary->op, left, right);   
            return LoxObject(std::in_place_type<double>, std::get<double>(left) * std::get<double>(right));

        case PLUS:
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return LoxObject(std::in_place_type<double>, std::get<double>(left) + std::get<double>(right));
            if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) return LoxObject(std::in_place_type<std::string>, std::get<std::string>(left) + std::get<std::string>(right));
            throw RuntimeError(*binary->op, "Operands must be two numbers or two strings.");
    }

    return LoxObject(std::in_place_type<void*>, nullptr);
}

LoxObject Interpreter::visitVariable(Variable* var){
    return environment->get(*(var->name));
}

void Interpreter::visitExpression(Expression* stmt){
    evaluate(stmt->expression.get());
}

void Interpreter::visitIf(If* stmt){
    if (isTruthy(evaluate(stmt->condition.get()))) {
        execute(stmt->thenBranch.get());
    } else if (stmt->elseBranch){
        execute(stmt->elseBranch.get());
    }
}

void Interpreter::visitPrint(Print* stmt){
    LoxObject value = evaluate(stmt->expression.get());
    std::cout << stringify(value) << std::endl;
}

void Interpreter::visitVar(Var* stmt){
    LoxObject value = LoxObject(std::in_place_type<void*>, nullptr);
    if (stmt->initializer) {
        value = evaluate(stmt->initializer.get());
    }

    environment->define(stmt->name->lexeme, value);
}

void Interpreter::visitBlock(Block* stmt){
    executeBlock(std::move(stmt->statements), std::make_unique<Environment>(environment.get()));
}

void Interpreter::executeBlock(std::vector<std::unique_ptr<Stmt>> statements, std::unique_ptr<Environment> environment){
    std::unique_ptr<Environment> previous = std::move(this->environment);
    this->environment = std::move(environment);
    auto cleanup = [this, &previous](){
        this->environment = std::move(previous);
    };

    struct ScopeGuard{
        std::function<void()> rollback;
        ~ScopeGuard() { rollback(); }
    } guard{cleanup};

    for (const auto& statement : statements){
        execute(statement.get());
    }
}

void Interpreter::checkNumberOperand(Token op, LoxObject operand){
    if (std::holds_alternative<double>(operand)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

bool Interpreter::isTruthy(LoxObject object){
    if (std::holds_alternative<void*>(object)) return false;
    if (std::holds_alternative<bool>(object)) return std::get<bool>(object);
    return true;
}

bool Interpreter::isEqual(LoxObject a, LoxObject b){
    return a == b;
}

void Interpreter::checkNumberOperands(Token op, LoxObject left, LoxObject right){
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>> statements){
    try {
        for (const std::unique_ptr<Stmt>& statement : statements) {
            execute(statement.get());
        }
    } catch (RuntimeError error) {
        Loxi::runtimeError(error);
    }
}

void Interpreter::execute(Stmt* stmt){
    stmt->acceptInterpreter(this);
}

std::string Interpreter::stringify(LoxObject object){
    if (std::holds_alternative<void*>(object)) return "nil";
    if (std::holds_alternative<double>(object)) return std::to_string(std::get<double>(object));
    if (std::holds_alternative<bool>(object)) return (std::get<bool>(object) ? "true" : "false");
    if (std::holds_alternative<std::string>(object)) return std::get<std::string>(object);
}