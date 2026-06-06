#pragma once
#include "Expr.hpp"

class Interpreter{
    LoxObject evaluate(Expr* expression);
    bool isTruthy(LoxObject object);
    bool isEqual(LoxObject a, LoxObject b);

public:
    Interpreter() = default;
    LoxObject visitBinary(Binary* binary);
    LoxObject visitGrouping(Grouping* grouping);
    LoxObject visitLiteral(Literal* literal);
    LoxObject visitUnary(Unary* unary);
};