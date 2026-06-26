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

