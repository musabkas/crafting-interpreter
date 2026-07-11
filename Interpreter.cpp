#include "Interpreter.hpp"
#include "RuntimeError.hpp"
#include "LoxCallable.hpp"
#include "LoxFunction.hpp"
#include "LoxClass.hpp"
#include "LoxInstance.hpp"
#include "Loxi.hpp"
#include <iostream>

Interpreter::Interpreter(){
    environment = std::make_shared<Environment>();
    globals = environment;
    locals = {};

    struct ClockCallable : LoxCallable {
        int arity() override { return 0; }
        LoxObject call(Interpreter* interpreter, std::vector<LoxObject> arguments) override {
            auto now = std::chrono::system_clock::now().time_since_epoch();
            return LoxObject(std::in_place_type<double>, std::chrono::duration_cast<std::chrono::milliseconds>(now).count() / 1000.0);
        };
        std::string toString() { return "<native fn>"; }
    };

    globals->define("clock", LoxObject(std::in_place_type<std::shared_ptr<LoxCallable>>, std::make_shared<ClockCallable>()));
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
    if (locals.count(assign)) {
        environment->assignAt(locals[assign], *(assign->name), value);
    } else {
        globals->assign(*(assign->name), value);
    }
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

LoxObject Interpreter::visitCall(Call* call){
    LoxObject callee = evaluate(call->callee.get());
    std::vector<LoxObject> arguments = {};
    for (auto& argument : call->arguments){
        arguments.emplace_back(evaluate(argument.get()));
    }

    if (!(std::holds_alternative<std::shared_ptr<LoxCallable>>(callee))) {
        throw RuntimeError(*call->paren, "Can only call functions and classes.");
    }
    std::shared_ptr<LoxCallable> function = std::get<std::shared_ptr<LoxCallable>>(callee);
    if (arguments.size() != function->arity()){
        throw RuntimeError(*call->paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
    }
    return function->call(this, arguments);
}

LoxObject Interpreter::visitGet(Get* get){
    LoxObject object = evaluate(get->object.get());
    if (std::holds_alternative<std::shared_ptr<LoxInstance>>(object)){
        return std::get<std::shared_ptr<LoxInstance>>(object)->get(get->name.get());
    }

    throw RuntimeError(*get->name, "Only instances have properties.");
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

LoxObject Interpreter::visitSet(Set* set){
    LoxObject object = evaluate(set->object.get());

    if (!(std::holds_alternative<std::shared_ptr<LoxInstance>>(object))){
        throw RuntimeError(*set->name, "Only instances have fields.");
    }

    LoxObject value = evaluate(set->value.get());
    std::get<std::shared_ptr<LoxInstance>>(object)->set(set->name.get(), value);
    return value;
}

LoxObject Interpreter::visitThis(This* thisExpr){
    return lookUpVariable(*(thisExpr->keyword), thisExpr);
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
    return lookUpVariable(*var->name, var);
}

LoxObject Interpreter::lookUpVariable(Token name, Expr* expr){
    if (locals.count(expr)) {
        return environment->getAt(locals[expr], name.lexeme);
    } else {
        return globals->get(name);
    }
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

void Interpreter::visitWhile(While* stmt){
    while (isTruthy(evaluate(stmt->condition.get()))) {
        execute(stmt->body.get());
    }
}

void Interpreter::visitReturn(Return* stmt){
    LoxObject value = LoxObject(std::in_place_type<void*>, nullptr);
    if (stmt->value != nullptr) value = evaluate(stmt->value.get());
    throw ReturnException(value);
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

void Interpreter::visitFunction(Function* stmt){
    LoxFunction function = LoxFunction(stmt, environment, false);
    environment->define(stmt->name->lexeme, LoxObject(std::in_place_type<std::shared_ptr<LoxCallable>>, std::make_shared<LoxFunction>(function)));
}

void Interpreter::visitClass(Class* stmt){
    environment->define(stmt->name->lexeme, LoxObject(std::in_place_type<void*>, nullptr));
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods = {};
    for (auto& method : stmt->methods){
        std::shared_ptr<LoxFunction> function = std::make_shared<LoxFunction>(method.get(), environment, method->name->lexeme == "init");
        methods[method->name->lexeme] = function;
    }
    std::shared_ptr<LoxClass> klass = std::make_shared<LoxClass>(stmt->name->lexeme, methods);
    environment->assign(*(stmt->name), LoxObject(std::in_place_type<std::shared_ptr<LoxCallable>>, klass));
}

void Interpreter::visitBlock(Block* stmt){
    executeBlock(stmt->statements, std::make_shared<Environment>(environment));
}

void Interpreter::executeBlock(std::vector<std::unique_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment){
    std::shared_ptr<Environment> previous = this->environment;
    this->environment = environment;
    auto cleanup = [this, &previous](){
        this->environment = previous;
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

void Interpreter::resolve(Expr* expr, int depth){
    locals[expr] = depth;
}

std::string Interpreter::stringify(LoxObject object){
    if (std::holds_alternative<void*>(object)) return "nil";
    if (std::holds_alternative<double>(object)) return std::to_string(std::get<double>(object));
    if (std::holds_alternative<bool>(object)) return (std::get<bool>(object) ? "true" : "false");
    if (std::holds_alternative<std::string>(object)) return std::get<std::string>(object);
    if (std::holds_alternative<std::shared_ptr<LoxCallable>>(object)) return std::get<std::shared_ptr<LoxCallable>>(object)->toString();
    if (std::holds_alternative<std::shared_ptr<LoxInstance>>(object)) return std::get<std::shared_ptr<LoxInstance>>(object)->toString();
}