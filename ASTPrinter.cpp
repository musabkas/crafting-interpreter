// #include "ASTPrinter.hpp"
// #include <iostream>

// std::string ASTPrinter::print(Expr* expr) {
//     return expr->acceptASTPrinter(this);
// }

// std::string ASTPrinter::visitBinary(Binary* binary) {
//     return parenthesize(binary->op->lexeme, {binary->left.get(), binary->right.get()});
// }
// std::string ASTPrinter::visitGrouping(Grouping* grouping) {
//     return parenthesize("group", {grouping->expression.get()});
// }
// std::string ASTPrinter::visitLiteral(Literal* literal) {
//     if (std::holds_alternative<double>(literal->value)) return std::to_string((std::get<double>(literal->value)));
//     if (std::holds_alternative<std::string>(literal->value)) return std::get<std::string>(literal->value);
//     if (std::holds_alternative<bool>(literal->value)) return (std::get<bool>(literal->value)) ? "true" : "false";
//     else return "nil";
// }

// std::string ASTPrinter::visitUnary(Unary* unary) {
//     return parenthesize(unary->op->lexeme, {unary->right.get()});
// }

// std::string ASTPrinter::parenthesize(std::string name, std::vector<Expr*> exprs){
//     std::string s;
//     s += "(" + name;
//     for (Expr* expr : exprs) {
//         s += " " + expr->acceptASTPrinter(this);
//     }
//     s += ")";
//     return s;
// }

// // int main(){
// //     std::unique_ptr<Binary> expression = std::make_unique<Binary>(std::make_unique<Unary>(std::make_unique<Token>(MINUS, "-", nullptr, 1),
// //                                     std::make_unique<Literal>(123.0)),
// //                                 std::make_unique<Token>(STAR, "*", nullptr, 1),
// //                                 std::make_unique<Grouping>(std::make_unique<Literal>(45.67)));
// //     ASTPrinter AST;
// //     std::cout << AST.print(expression.get()) << std::endl;
// // }