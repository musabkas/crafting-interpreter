#pragma once
#include "LoxCallable.hpp"
#include "Expr.hpp"

class LoxFunction : public LoxCallable {
private:
    Function* declaration;
public:
    LoxFunction(Function* declaration);
    int arity() override;
    LoxObject call(Interpreter* interpreter, std::vector<LoxObject> arguments) override;
    std::string toString() override;
};