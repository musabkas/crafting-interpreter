#include "Expr.hpp"
#include "Interpreter.hpp"
#include "Resolver.hpp"

Assign::Assign(std::unique_ptr<Token> name, std::unique_ptr<Expr> value){
	this->name = std::move(name);
	this->value = std::move(value);
}

LoxObject Assign::acceptInterpreter(Interpreter* visitor){
	return visitor->visitAssign(this);
}

void Assign::acceptResolver(Resolver* visitor){
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

void Logical::acceptResolver(Resolver* visitor){
	return visitor->visitLogical(this);
}

Set::Set(std::unique_ptr<Expr> object, std::unique_ptr<Token> name, std::unique_ptr<Expr> value){
	this->object = std::move(object);
	this->name = std::move(name);
	this->value = std::move(value);
}

LoxObject Set::acceptInterpreter(Interpreter* visitor){
	return visitor->visitSet(this);
}

void Set::acceptResolver(Resolver* visitor){
	return visitor->visitSet(this);
}

Super::Super(std::unique_ptr<Token> keyword, std::unique_ptr<Token> method){
	this->keyword = std::move(keyword);
	this->method = std::move(method);
}

LoxObject Super::acceptInterpreter(Interpreter* visitor){
	return visitor->visitSuper(this);
}

void Super::acceptResolver(Resolver* visitor){
	return visitor->visitSuper(this);
}

This::This(std::unique_ptr<Token> keyword){
	this->keyword = std::move(keyword);
}

LoxObject This::acceptInterpreter(Interpreter* visitor){
	return visitor->visitThis(this);
}

void This::acceptResolver(Resolver* visitor){
	return visitor->visitThis(this);
}

Binary::Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right){
	this->left = std::move(left);
	this->op = std::move(op);
	this->right = std::move(right);
}

LoxObject Binary::acceptInterpreter(Interpreter* visitor){
	return visitor->visitBinary(this);
}

void Binary::acceptResolver(Resolver* visitor){
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

void Call::acceptResolver(Resolver* visitor){
	return visitor->visitCall(this);
}

Get::Get(std::unique_ptr<Expr> object, std::unique_ptr<Token> name){
	this->object = std::move(object);
	this->name = std::move(name);
}

LoxObject Get::acceptInterpreter(Interpreter* visitor){
	return visitor->visitGet(this);
}

void Get::acceptResolver(Resolver* visitor){
	return visitor->visitGet(this);
}

Grouping::Grouping(std::unique_ptr<Expr> expression){
	this->expression = std::move(expression);
}

LoxObject Grouping::acceptInterpreter(Interpreter* visitor){
	return visitor->visitGrouping(this);
}

void Grouping::acceptResolver(Resolver* visitor){
	return visitor->visitGrouping(this);
}

Literal::Literal(LoxObject value){
	this->value = value;
}

LoxObject Literal::acceptInterpreter(Interpreter* visitor){
	return visitor->visitLiteral(this);
}

void Literal::acceptResolver(Resolver* visitor){
	return visitor->visitLiteral(this);
}

Unary::Unary(std::unique_ptr<Token> op, std::unique_ptr<Expr> right){
	this->op = std::move(op);
	this->right = std::move(right);
}

LoxObject Unary::acceptInterpreter(Interpreter* visitor){
	return visitor->visitUnary(this);
}

void Unary::acceptResolver(Resolver* visitor){
	return visitor->visitUnary(this);
}

Variable::Variable(std::unique_ptr<Token> name){
	this->name = std::move(name);
}

LoxObject Variable::acceptInterpreter(Interpreter* visitor){
	return visitor->visitVariable(this);
}

void Variable::acceptResolver(Resolver* visitor){
	return visitor->visitVariable(this);
}

