#pragma once
#include "Expr.hpp"

class LoxClass {
private:
    std::string name;
public:
    LoxClass(std::string name);
    std::string toString();
};