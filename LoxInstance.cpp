#include "LoxInstance.hpp"

LoxInstance::LoxInstance(LoxClass* klass){
    this->klass = klass;
    this->fields = {};
}

LoxObject LoxInstance::get(Token* name){
    if (fields.count(name->lexeme)) {
        return fields[name->lexeme];
    }

    std::shared_ptr<LoxFunction> method = klass->findMethod(name->lexeme);
    if (method != nullptr) return LoxObject(std::in_place_type<std::shared_ptr<LoxCallable>>, method->bind(shared_from_this()));

    throw RuntimeError(*name, "Undefined property '" + name->lexeme + "'.");
}

void LoxInstance::set(Token* name, LoxObject value){
    fields[name->lexeme] = value;
}

std::string LoxInstance::toString(){
    return klass->name + " instance";
}