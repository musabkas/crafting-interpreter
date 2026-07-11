#pragma once
#include "Expr.hpp"
#include "LoxClass.hpp"

class LoxInstance{
    LoxClass* klass;
public:
    LoxInstance(LoxClass* klass);
    std::string toString();
};