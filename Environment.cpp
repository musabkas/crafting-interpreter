#include "Environment.hpp"
#include <iostream>

Environment::Environment(){
    enclosing = nullptr;
}

Environment::Environment(std::shared_ptr<Environment> enclosing){
    this->enclosing = enclosing;
}

void Environment::define(std::string name, LoxObject value){
    values[name] = value;
}

void Environment::assign(Token name, LoxObject value){
    if (values.count(name.lexeme)){
        values[name.lexeme] = value;
    } else if (enclosing != nullptr) {
        enclosing->assign(name, value);
    } else {
        throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
}

void Environment::assignAt(int distance, Token name, LoxObject value){
    ancestor(distance)->values[name.lexeme] = value;
}

LoxObject Environment::get(Token name){
    if (values.count(name.lexeme)){
        return values[name.lexeme];
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

LoxObject Environment::getAt(int distance, std::string name){
    return ancestor(distance)->values[name];
}

Environment* Environment::ancestor(int distance){
    Environment* environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing.get();
    }
    return environment;
}