#pragma once
#include "Token.hpp"
#include <string>

class ASTPrinter;

class Expr {
public:
	virtual std::string acceptASTPrinter(ASTPrinter* visitor) = 0;
};

class Binary : public Expr {
public:
	Expr* left;
	Token* op;
	Expr* right;

	Binary(Expr* left, Token* op, Expr* right);
	std::string acceptASTPrinter(ASTPrinter* visitor) override;
};

class Grouping : public Expr {
public:
	Expr* expression;

	Grouping(Expr* expression);
	std::string acceptASTPrinter(ASTPrinter* visitor) override;
};

class Literal : public Expr {
public:
	std::variant<double, std::string, void *> value;

	Literal(std::variant<double, std::string, void *> value);
	std::string acceptASTPrinter(ASTPrinter* visitor) override;
};

class Unary : public Expr {
public:
	Token* op;
	Expr* right;

	Unary(Token* op, Expr* right);
	std::string acceptASTPrinter(ASTPrinter* visitor) override;
};

