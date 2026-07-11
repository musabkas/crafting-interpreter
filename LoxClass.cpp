#include "LoxClass.hpp"
#include "LoxInstance.hpp"

LoxClass::LoxClass(std::string name){
    this->name = name;
}

std::string LoxClass::toString(){
    return name;
}

LoxObject LoxClass::call(Interpreter* interpreter, std::vector<LoxObject> arguments) {
    std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(this);
    return LoxObject(std::in_place_type<std::shared_ptr<LoxInstance>>, instance);
}

int LoxClass::arity(){
    return 0;
}