#pragma once
#include <unordered_map>
#include "Expr.hpp"
#include "RuntimeError.hpp"

class Environment{
    std::unordered_map<std::string, LoxObject> values = {};
    Environment* ancestor(int distance);
public:
    std::shared_ptr<Environment> enclosing;
    Environment();
    Environment(std::shared_ptr<Environment> enclosing);
    void define(std::string name, LoxObject value);
    void assign(Token name, LoxObject value);
    void assignAt(int distance, Token name, LoxObject value);
    LoxObject get(Token name);
    LoxObject getAt(int distance, std::string name);
};