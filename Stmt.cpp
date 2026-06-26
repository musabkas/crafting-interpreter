#include "Stmt.hpp"
#include "Interpreter.hpp"

Expression::Expression(std::unique_ptr<Expr> expression){
	this->expression = std::move(expression);
}

void Expression::acceptInterpreter(Interpreter* visitor){
	return visitor->visitExpression(this);
}

Print::Print(std::unique_ptr<Expr> expression){
	this->expression = std::move(expression);
}

void Print::acceptInterpreter(Interpreter* visitor){
	return visitor->visitPrint(this);
}

Var::Var(std::unique_ptr<Token> name, std::unique_ptr<Expr> initializer){
	this->name = std::move(name);
	this->initializer = std::move(initializer);
}

void Var::acceptInterpreter(Interpreter* visitor){
	return visitor->visitVar(this);
}

