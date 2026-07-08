#pragma once
#include "Expr.hpp"

class ReturnException : public std::runtime_error {
public:
    LoxObject value;
    ReturnException(LoxObject value) : std::runtime_error(""), value(value) { }
};