#pragma once
#include "Token.hpp"
#include <string>
#include <memory>
#include "Expr.hpp"
using LoxObject = std::variant<double, std::string, bool, void*>;

class Interpreter;

class Stmt {
public:
	virtual ~Stmt() = default;
	virtual void acceptInterpreter(Interpreter* visitor) = 0;
};

class Expression : public Stmt {
public:
	std::unique_ptr<Expr> expression;

	Expression(std::unique_ptr<Expr> expression);
	void acceptInterpreter(Interpreter* visitor) override;
};

class Print : public Stmt {
public:
	std::unique_ptr<Expr> expression;

	Print(std::unique_ptr<Expr> expression);
	void acceptInterpreter(Interpreter* visitor) override;
};

