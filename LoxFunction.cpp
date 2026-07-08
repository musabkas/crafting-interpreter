#include "LoxFunction.hpp"

LoxFunction::LoxFunction(Function* declaration, std::shared_ptr<Environment> closure){
    this->declaration = declaration;
    this->closure = closure;
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