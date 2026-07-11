#include "LoxInstance.hpp"

LoxInstance::LoxInstance(LoxClass* klass){
    this->klass = klass;
    this->fields = {};
}

LoxObject LoxInstance::get(Token* name){
    if (fields.count(name->lexeme)) {
        return fields[name->lexeme];
    }

    throw RuntimeError(*name, "Undefined property '" + name->lexeme + "'.");
}

void LoxInstance::set(Token* name, LoxObject value){
    fields[name->lexeme] = value;
}

std::string LoxInstance::toString(){
    return klass->name + " instance";
}