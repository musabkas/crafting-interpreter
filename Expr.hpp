#pragma once
#include "Token.hpp"
#include <string>
#include <memory>

using LoxObject = std::variant<double, std::string, bool, void*>;

class ASTPrinter;

class Expr {
public:
	virtual ~Expr() = default;
	virtual std::string acceptASTPrinter(ASTPrinter* visitor) = 0;
};

class Binary : public Expr {
public:
	std::unique_ptr<Expr> left;
	std::unique_ptr<Token> op;
	std::unique_ptr<Expr> right;

	Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right);
	std::string acceptASTPrinter(ASTPrinter* visitor) override;
};

class Grouping : public Expr {
public:
	std::unique_ptr<Expr> expression;

	Grouping(std::unique_ptr<Expr> expression);
	std::string acceptASTPrinter(ASTPrinter* visitor) override;
};

class Literal : public Expr {
public:
	LoxObject value;

	Literal(LoxObject value);
	std::string acceptASTPrinter(ASTPrinter* visitor) override;
};

class Unary : public Expr {
public:
	std::unique_ptr<Token> op;
	std::unique_ptr<Expr> right;

	Unary(std::unique_ptr<Token> op, std::unique_ptr<Expr> right);
	std::string acceptASTPrinter(ASTPrinter* visitor) override;
};

