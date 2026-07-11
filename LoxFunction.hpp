#pragma once
#include "LoxCallable.hpp"
#include "Expr.hpp"

class LoxFunction : public LoxCallable {
private:
    Function* declaration;
    std::shared_ptr<Environment> closure;
public:
    LoxFunction(Function* declaration, std::shared_ptr<Environment> closure);
    std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> instance);
    int arity() override;
    LoxObject call(Interpreter* interpreter, std::vector<LoxObject> arguments) override;
    std::string toString() override;
};