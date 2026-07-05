#pragma once
#include "Token.hpp"
#include <string>
#include <memory>

using LoxObject = std::variant<double, std::string, bool, void*>;

class Interpreter;

class Expr {
public:
	virtual ~Expr() = default;
	virtual LoxObject acceptInterpreter(Interpreter* visitor) = 0;
};

class Assign : public Expr {
public:
	std::unique_ptr<Token> name;
	std::unique_ptr<Expr> value;

	Assign(std::unique_ptr<Token> name, std::unique_ptr<Expr> value);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
};

class Logical : public Expr {
public:
	std::unique_ptr<Expr> left;
	std::unique_ptr<Token> op;
	std::unique_ptr<Expr> right;

	Logical(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
};

class Binary : public Expr {
public:
	std::unique_ptr<Expr> left;
	std::unique_ptr<Token> op;
	std::unique_ptr<Expr> right;

	Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
};

class Grouping : public Expr {
public:
	std::unique_ptr<Expr> expression;

	Grouping(std::unique_ptr<Expr> expression);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
};

class Literal : public Expr {
public:
	LoxObject value;

	Literal(LoxObject value);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
};

class Unary : public Expr {
public:
	std::unique_ptr<Token> op;
	std::unique_ptr<Expr> right;

	Unary(std::unique_ptr<Token> op, std::unique_ptr<Expr> right);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
};

class Variable : public Expr {
public:
	std::unique_ptr<Token> name;

	Variable(std::unique_ptr<Token> name);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
};

