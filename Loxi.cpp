#include <fstream>
#include <iostream>

#include "Scanner.hpp"
#include "Parser.hpp"
#include "Loxi.hpp"
#include "ASTPrinter.hpp"

bool Loxi::hadError = false;

void Loxi::run(std::string source) {
    std::unique_ptr<Scanner> scanner = std::make_unique<Scanner>(source);
    std::vector<std::unique_ptr<Token>> tokens = scanner->scanTokens();

    Parser parser (std::move(tokens));
    std::unique_ptr<Expr> expression = std::move(parser.parse());

    if (hadError) return;
    
    ASTPrinter AST;
    std::cout << AST.print(expression.get()) << std::endl;
}
void Loxi::error(int line, std::string message){
    report(line, "", message);
}

void Loxi::report(int line, std::string where, std::string message){
    std::cout << "[line " << line << "] Error" + where + ": " + message << std::endl;
    hadError = true;
}

void Loxi::error(Token token, std::string message){
    if (token.type == END_OF_FILE) {
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}


void Loxi::runPrompt() {
    while (true) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) break; // if EOF sent then end program, else read line
        run(line);
        hadError = false;
    }
    std::cout << std::endl;
}

int Loxi::runScript(std::string path){
    std::ifstream script(path);
    if (!script.is_open()) {
        std::cout << "Could not open file: " << path << std::endl;
        return 1;
    }

    std::string source;
    std::string line;
    while (std::getline(script, line)) {
        source += line + "\n";
    }
    run(source);

    if (hadError) return 1; // Non-zero exit code for error
    else return 0;
}


int main(int argc, char* argv[]){
    Loxi program = Loxi();
    if (argc > 2) {
        std::cout << "Usage: loxi [script]" << std::endl;
    } else if (argc == 2) {
        std::cout << "Running script " << argv[1] << std::endl;
        return program.runScript(argv[1]);
    } else {
        std::cout << "Running prompt " << std::endl;
        program.runPrompt();
    }
}
