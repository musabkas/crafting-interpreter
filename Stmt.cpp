#include "Stmt.hpp"
#include "Interpreter.hpp"

Expression::Expression(std::unique_ptr<Expr> expression){
	this->expression = std::move(expression);
}

void Expression::acceptInterpreter(Interpreter* visitor){
	return visitor->visitExpression(this);
}

If::If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch){
	this->condition = std::move(condition);
	this->thenBranch = std::move(thenBranch);
	this->elseBranch = std::move(elseBranch);
}

void If::acceptInterpreter(Interpreter* visitor){
	return visitor->visitIf(this);
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

Function::Function(std::unique_ptr<Token> name, std::vector<std::unique_ptr<Token>> params, std::vector<std::unique_ptr<Stmt>> body){
	this->name = std::move(name);
	this->params = std::move(params);
	this->body = std::move(body);
}

void Function::acceptInterpreter(Interpreter* visitor){
	return visitor->visitFunction(this);
}

While::While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body){
	this->condition = std::move(condition);
	this->body = std::move(body);
}

void While::acceptInterpreter(Interpreter* visitor){
	return visitor->visitWhile(this);
}

Block::Block(std::vector<std::unique_ptr<Stmt>> statements){
	this->statements = std::move(statements);
}

void Block::acceptInterpreter(Interpreter* visitor){
	return visitor->visitBlock(this);
}

