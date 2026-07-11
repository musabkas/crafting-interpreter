#pragma once
#include "Token.hpp"
#include <string>
#include <memory>
#include <vector>

class LoxCallable;
class LoxInstance;
using LoxObject = std::variant<double, std::string, bool, std::shared_ptr<LoxCallable>, std::shared_ptr<LoxInstance>, void*>;

class Interpreter;
class Resolver;

class Expr {
public:
	virtual ~Expr() = default;
	virtual LoxObject acceptInterpreter(Interpreter* visitor) = 0;
	virtual void acceptResolver(Resolver* visitor) = 0;
};

class Assign : public Expr {
public:
	std::unique_ptr<Token> name;
	std::unique_ptr<Expr> value;

	Assign(std::unique_ptr<Token> name, std::unique_ptr<Expr> value);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Logical : public Expr {
public:
	std::unique_ptr<Expr> left;
	std::unique_ptr<Token> op;
	std::unique_ptr<Expr> right;

	Logical(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Set : public Expr {
public:
	std::unique_ptr<Expr> object;
	std::unique_ptr<Token> name;
	std::unique_ptr<Expr> value;

	Set(std::unique_ptr<Expr> object, std::unique_ptr<Token> name, std::unique_ptr<Expr> value);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class This : public Expr {
public:
	std::unique_ptr<Token> keyword;

	This(std::unique_ptr<Token> keyword);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Binary : public Expr {
public:
	std::unique_ptr<Expr> left;
	std::unique_ptr<Token> op;
	std::unique_ptr<Expr> right;

	Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Call : public Expr {
public:
	std::unique_ptr<Expr> callee;
	std::unique_ptr<Token> paren;
	std::vector<std::unique_ptr<Expr>> arguments;

	Call(std::unique_ptr<Expr> callee, std::unique_ptr<Token> paren, std::vector<std::unique_ptr<Expr>> arguments);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Get : public Expr {
public:
	std::unique_ptr<Expr> object;
	std::unique_ptr<Token> name;

	Get(std::unique_ptr<Expr> object, std::unique_ptr<Token> name);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Grouping : public Expr {
public:
	std::unique_ptr<Expr> expression;

	Grouping(std::unique_ptr<Expr> expression);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Literal : public Expr {
public:
	LoxObject value;

	Literal(LoxObject value);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Unary : public Expr {
public:
	std::unique_ptr<Token> op;
	std::unique_ptr<Expr> right;

	Unary(std::unique_ptr<Token> op, std::unique_ptr<Expr> right);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Variable : public Expr {
public:
	std::unique_ptr<Token> name;

	Variable(std::unique_ptr<Token> name);
	LoxObject acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

