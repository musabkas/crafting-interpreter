#pragma once
#include "Expr.hpp"
#include "Interpreter.hpp"

class LoxCallable{
public:
    virtual ~LoxCallable() = default;
    
    virtual int arity() = 0;
    virtual LoxObject call(Interpreter* interpreter, std::vector<LoxObject> arguments) = 0;
    virtual std::string toString() = 0;
};