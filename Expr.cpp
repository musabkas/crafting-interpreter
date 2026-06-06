#include "Expr.hpp"
#include "ASTPrinter.hpp"
#include "Interpreter.hpp"

Binary::Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right){
	this->left = std::move(left);
	this->op = std::move(op);
	this->right = std::move(right);
}

std::string Binary::acceptASTPrinter(ASTPrinter* visitor){
	return visitor->visitBinary(this);
}

LoxObject Binary::acceptInterpreter(Interpreter* visitor){
	return visitor->visitBinary(this);
}

Grouping::Grouping(std::unique_ptr<Expr> expression){
	this->expression = std::move(expression);
}

std::string Grouping::acceptASTPrinter(ASTPrinter* visitor){
	return visitor->visitGrouping(this);
}

LoxObject Grouping::acceptInterpreter(Interpreter* visitor){
	return visitor->visitGrouping(this);
}

Literal::Literal(LoxObject value){
	this->value = value;
}

std::string Literal::acceptASTPrinter(ASTPrinter* visitor){
	return visitor->visitLiteral(this);
}

LoxObject Literal::acceptInterpreter(Interpreter* visitor){
	return visitor->visitLiteral(this);
}

Unary::Unary(std::unique_ptr<Token> op, std::unique_ptr<Expr> right){
	this->op = std::move(op);
	this->right = std::move(right);
}

std::string Unary::acceptASTPrinter(ASTPrinter* visitor){
	return visitor->visitUnary(this);
}

LoxObject Unary::acceptInterpreter(Interpreter* visitor){
	return visitor->visitUnary(this);
}

