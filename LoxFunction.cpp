#include "LoxFunction.hpp"

LoxFunction::LoxFunction(Function* declaration){
    this->declaration = declaration;
}

LoxObject LoxFunction::call(Interpreter* interpreter, std::vector<LoxObject> arguments) {
    std::unique_ptr<Environment> environment = std::make_unique<Environment>(*(interpreter->globals));
    for (int i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i]->lexeme, arguments[i]);
    }
    try {
        interpreter->executeBlock(declaration->body, std::move(environment));
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