#include "Expr.hpp"
#include "ASTPrinter.hpp"

Binary::Binary(Expr* left, Token* op, Expr* right){
	this->left = left;
	this->op = op;
	this->right = right;
}

std::string Binary::acceptASTPrinter(ASTPrinter* visitor){
	return visitor->visitBinary(this);
}

Grouping::Grouping(Expr* expression){
	this->expression = expression;
}

std::string Grouping::acceptASTPrinter(ASTPrinter* visitor){
	return visitor->visitGrouping(this);
}

Literal::Literal(std::variant<double, std::string, void *> value){
	this->value = value;
}

std::string Literal::acceptASTPrinter(ASTPrinter* visitor){
	return visitor->visitLiteral(this);
}

Unary::Unary(Token* op, Expr* right){
	this->op = op;
	this->right = right;
}

std::string Unary::acceptASTPrinter(ASTPrinter* visitor){
	return visitor->visitUnary(this);
}

