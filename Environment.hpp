#pragma once
#include <unordered_map>
#include "Expr.hpp"
#include "RuntimeError.hpp"

class Environment{
    std::unordered_map<std::string, LoxObject> values = {};
    
public:
    void define(std::string name, LoxObject value);
    void assign(Token name, LoxObject value);
    LoxObject get(Token name);
};