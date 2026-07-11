#pragma once
#include "Expr.hpp"
#include "LoxClass.hpp"

class LoxInstance : public std::enable_shared_from_this<LoxInstance> {
    LoxClass* klass;
    std::unordered_map<std::string, LoxObject> fields;
public:
    LoxInstance(LoxClass* klass);
    void set(Token* name, LoxObject value);
    LoxObject get(Token* name);
    std::string toString();
};