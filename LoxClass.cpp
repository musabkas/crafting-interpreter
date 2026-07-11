#include "LoxClass.hpp"

LoxClass::LoxClass(std::string name){
    this->name = name;
}

std::string LoxClass::toString(){
    return name;
}