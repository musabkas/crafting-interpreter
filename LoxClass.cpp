#include "LoxClass.hpp"
#include "LoxInstance.hpp"

LoxClass::LoxClass(std::string name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods){
    this->name = name;
    this->methods = methods;
}

std::shared_ptr<LoxFunction> LoxClass::findMethod(std::string name){
    if (methods.count(name)){
        return methods[name];
    }
    return nullptr;
}

std::string LoxClass::toString(){
    return name;
}

LoxObject LoxClass::call(Interpreter* interpreter, std::vector<LoxObject> arguments) {
    std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(this);
    std::shared_ptr<LoxFunction> initializer = findMethod("init");
    if (initializer != nullptr) {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return LoxObject(std::in_place_type<std::shared_ptr<LoxInstance>>, instance);
}

int LoxClass::arity(){
    std::shared_ptr<LoxFunction> initializer = findMethod("init");
    if (initializer == nullptr) return 0;
    return initializer->arity();
}