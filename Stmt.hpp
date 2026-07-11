#pragma once
#include "Token.hpp"
#include <string>
#include <memory>
#include <vector>
#include "Expr.hpp"

class Interpreter;
class Resolver;

class Stmt {
public:
	virtual ~Stmt() = default;
	virtual void acceptInterpreter(Interpreter* visitor) = 0;
	virtual void acceptResolver(Resolver* visitor) = 0;
};

class Expression : public Stmt {
public:
	std::unique_ptr<Expr> expression;

	Expression(std::unique_ptr<Expr> expression);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class If : public Stmt {
public:
	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> thenBranch;
	std::unique_ptr<Stmt> elseBranch;

	If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Print : public Stmt {
public:
	std::unique_ptr<Expr> expression;

	Print(std::unique_ptr<Expr> expression);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Return : public Stmt {
public:
	std::unique_ptr<Token> keyword;
	std::unique_ptr<Expr> value;

	Return(std::unique_ptr<Token> keyword, std::unique_ptr<Expr> value);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Var : public Stmt {
public:
	std::unique_ptr<Token> name;
	std::unique_ptr<Expr> initializer;

	Var(std::unique_ptr<Token> name, std::unique_ptr<Expr> initializer);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Function : public Stmt {
public:
	std::unique_ptr<Token> name;
	std::vector<std::unique_ptr<Token>> params;
	std::vector<std::unique_ptr<Stmt>> body;

	Function(std::unique_ptr<Token> name, std::vector<std::unique_ptr<Token>> params, std::vector<std::unique_ptr<Stmt>> body);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class While : public Stmt {
public:
	std::unique_ptr<Expr> condition;
	std::unique_ptr<Stmt> body;

	While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Block : public Stmt {
public:
	std::vector<std::unique_ptr<Stmt>> statements;

	Block(std::vector<std::unique_ptr<Stmt>> statements);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

class Class : public Stmt {
public:
	std::unique_ptr<Token> name;
	std::unique_ptr<Variable> superclass;
	std::vector<std::unique_ptr<Function>> methods;

	Class(std::unique_ptr<Token> name, std::unique_ptr<Variable> superclass, std::vector<std::unique_ptr<Function>> methods);
	void acceptInterpreter(Interpreter* visitor) override;
	void acceptResolver(Resolver* visitor) override;
};

