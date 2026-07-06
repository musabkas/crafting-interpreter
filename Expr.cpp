#include "Expr.hpp"
#include "Interpreter.hpp"

Assign::Assign(std::unique_ptr<Token> name, std::unique_ptr<Expr> value){
	this->name = std::move(name);
	this->value = std::move(value);
}

LoxObject Assign::acceptInterpreter(Interpreter* visitor){
	return visitor->visitAssign(this);
}

Logical::Logical(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right){
	this->left = std::move(left);
	this->op = std::move(op);
	this->right = std::move(right);
}

LoxObject Logical::acceptInterpreter(Interpreter* visitor){
	return visitor->visitLogical(this);
}

Binary::Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right){
	this->left = std::move(left);
	this->op = std::move(op);
	this->right = std::move(right);
}

LoxObject Binary::acceptInterpreter(Interpreter* visitor){
	return visitor->visitBinary(this);
}

Call::Call(std::unique_ptr<Expr> callee, std::unique_ptr<Token> paren, std::vector<std::unique_ptr<Expr>> arguments){
	this->callee = std::move(callee);
	this->paren = std::move(paren);
	this->arguments = std::move(arguments);
}

LoxObject Call::acceptInterpreter(Interpreter* visitor){
	return visitor->visitCall(this);
}

Grouping::Grouping(std::unique_ptr<Expr> expression){
	this->expression = std::move(expression);
}

LoxObject Grouping::acceptInterpreter(Interpreter* visitor){
	return visitor->visitGrouping(this);
}

Literal::Literal(LoxObject value){
	this->value = value;
}

LoxObject Literal::acceptInterpreter(Interpreter* visitor){
	return visitor->visitLiteral(this);
}

Unary::Unary(std::unique_ptr<Token> op, std::unique_ptr<Expr> right){
	this->op = std::move(op);
	this->right = std::move(right);
}

LoxObject Unary::acceptInterpreter(Interpreter* visitor){
	return visitor->visitUnary(this);
}

Variable::Variable(std::unique_ptr<Token> name){
	this->name = std::move(name);
}

LoxObject Variable::acceptInterpreter(Interpreter* visitor){
	return visitor->visitVariable(this);
}

