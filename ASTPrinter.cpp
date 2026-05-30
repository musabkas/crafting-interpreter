#include "ASTPrinter.hpp"
#include <iostream>

std::string ASTPrinter::print(Expr* expr) {
    return expr->acceptASTPrinter(this);
}

std::string ASTPrinter::visitBinary(Binary* binary) {
    return parenthesize(binary->op->lexeme, {binary->left, binary->right});
}
std::string ASTPrinter::visitGrouping(Grouping* grouping) {
    return parenthesize("group", {grouping->expression});
}
std::string ASTPrinter::visitLiteral(Literal* literal) {
    if (std::holds_alternative<double>(literal->value)) return std::to_string((std::get<double>(literal->value)));
    if (std::holds_alternative<std::string>(literal->value)) return std::get<std::string>(literal->value);
    else return "nil";
}

std::string ASTPrinter::visitUnary(Unary* unary) {
    return parenthesize(unary->op->lexeme, {unary->right});
}

std::string ASTPrinter::parenthesize(std::string name, std::vector<Expr*> exprs){
    std::string s;
    s += "(" + name;
    for (Expr* expr : exprs) {
        s += " " + expr->acceptASTPrinter(this);
    }
    s += ")";
    return s;
}

// int main(){
//     Expr* expression = new Binary(new Unary(new Token(MINUS, "-", nullptr, 1),
//                                     new Literal(123.0)),
//                                 new Token(STAR, "*", nullptr, 1),
//                                 new Grouping(new Literal (45.67)));
//     ASTPrinter AST;
//     std::cout << AST.print(expression) << std::endl;
// }