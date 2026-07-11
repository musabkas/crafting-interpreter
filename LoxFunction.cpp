#include "LoxFunction.hpp"

LoxFunction::LoxFunction(Function* declaration, std::shared_ptr<Environment> closure){
    this->declaration = declaration;
    this->closure = closure;
}

std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<LoxInstance>instance){
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    environment->define("this", LoxObject(std::in_place_type<std::shared_ptr<LoxInstance>>, instance));
    return std::make_shared<LoxFunction>(declaration, environment);
}

LoxObject LoxFunction::call(Interpreter* interpreter, std::vector<LoxObject> arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i]->lexeme, arguments[i]);
    }
    try {
        interpreter->executeBlock(declaration->body, environment);
    } catch (ReturnException returnValue) {
        return returnValue.value;
    }
    return LoxObject(std::in_place_type<void*>, nullptr);
}

int LoxFunction::arity(){
    return declaration->params.size();
}

std::string LoxFunction::toString(){
    return "<fn " + declaration->name->lexeme + ">";
}