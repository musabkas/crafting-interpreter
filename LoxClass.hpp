#pragma once
#include "LoxCallable.hpp"
#include "LoxFunction.hpp"
#include "Expr.hpp"

class LoxClass : public LoxCallable {
private:
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
public:
    std::string name;
    LoxClass(std::string name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods);
    std::shared_ptr<LoxFunction> findMethod(std::string name);
    std::string toString() override;
    int arity() override;
    LoxObject call(Interpreter* interpreter, std::vector<LoxObject> arguments) override;
};