#include "LoxInstance.hpp"

LoxInstance::LoxInstance(LoxClass* klass){
    this->klass = klass;
}

std::string LoxInstance::toString(){
    return klass->name + " instance";
}