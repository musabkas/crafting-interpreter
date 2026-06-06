#include "Interpreter.hpp"

LoxObject Interpreter::visitLiteral(Literal* literal){
    return literal->value;
}

LoxObject Interpreter::visitGrouping(Grouping* grouping){
    return evaluate(grouping->expression.get());
}

LoxObject Interpreter::evaluate(Expr* expression){
    return expression->acceptInterpreter(this);
}

LoxObject Interpreter::visitUnary(Unary* unary){
    LoxObject right = evaluate(unary->right.get());

    switch(unary->op->type) {
        case BANG:
            return LoxObject(std::in_place_type<bool>, !isTruthy(right));
        case MINUS:
            return LoxObject(std::in_place_type<double>, -std::get<double>(right));
    }

    return LoxObject(std::in_place_type<void*>, nullptr);
}

bool Interpreter::isTruthy(LoxObject object){
    if (std::holds_alternative<void*>(object)) return false;
    if (std::holds_alternative<bool>(object)) return std::get<bool>(object);
    return true;
}

LoxObject Interpreter::visitBinary(Binary* binary){
    LoxObject left = evaluate(binary->left.get());
    LoxObject right = evaluate(binary->right.get());

    switch (binary->op->type) {
        case GREATER: return LoxObject(std::in_place_type<bool>, std::get<double>(left) > std::get<double>(right));
        case GREATER_EQUAL: return LoxObject(std::in_place_type<bool>, std::get<double>(left) >= std::get<double>(right));
        case LESS: return LoxObject(std::in_place_type<bool>, std::get<double>(left) < std::get<double>(right));
        case LESS_EQUAL: return LoxObject(std::in_place_type<bool>, std::get<double>(left) <= std::get<double>(right));
        case BANG_EQUAL: return LoxObject(std::in_place_type<bool>, !isEqual(left, right));
        case EQUAL_EQUAL: return LoxObject(std::in_place_type<bool>, isEqual(left, right));

        case MINUS: return LoxObject(std::in_place_type<double>, std::get<double>(left) - std::get<double>(right));
        case SLASH: return LoxObject(std::in_place_type<double>, std::get<double>(left) / std::get<double>(right));
        case STAR: return LoxObject(std::in_place_type<double>, std::get<double>(left) * std::get<double>(right));

        case PLUS:
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return LoxObject(std::in_place_type<double>, std::get<double>(left) + std::get<double>(right));
            if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) return LoxObject(std::in_place_type<std::string>, std::get<std::string>(left) + std::get<std::string>(right));
            break;
    }

    return LoxObject(std::in_place_type<void*>, nullptr);
}

bool Interpreter::isEqual(LoxObject a, LoxObject b){
    return a == b;
}