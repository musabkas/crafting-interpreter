#include "Environment.hpp"

void Environment::define(std::string name, LoxObject value){
    values[name] = value;
}

void Environment::assign(Token name, LoxObject value){
    if (values.count(name.lexeme)){
        values[name.lexeme] = value;
    } else {
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
}

LoxObject Environment::get(Token name){
    if (values.count(name.lexeme)){
        return values[name.lexeme];
    }

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}