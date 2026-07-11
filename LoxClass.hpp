#pragma once
#include "LoxCallable.hpp"
#include "Expr.hpp"

class LoxClass : public LoxCallable {
public:
    std::string name;
    LoxClass(std::string name);
    std::string toString() override;
    int arity() override;
    LoxObject call(Interpreter* interpreter, std::vector<LoxObject> arguments) override;
};