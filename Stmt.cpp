#include "Stmt.hpp"
#include "Interpreter.hpp"

std::ostream& operator<<(std::ostream& os, const LoxObject& obj) {
	if (std::holds_alternative<double>(obj)) os << std::get<double>(obj);
	if (std::holds_alternative<std::string>(obj)) os << std::get<std::string>(obj);
	if (std::holds_alternative<bool>(obj)) os << std::get<bool>(obj);
	if (std::holds_alternative<void*>(obj)) os << "nil";
    return os;
}

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

Block::Block(std::vector<std::unique_ptr<Stmt>> statements){
	this->statements = std::move(statements);
}

void Block::acceptInterpreter(Interpreter* visitor){
	return visitor->visitBlock(this);
}

