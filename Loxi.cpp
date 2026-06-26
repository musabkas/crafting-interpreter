#include <fstream>
#include <iostream>

#include "Scanner.hpp"
#include "Parser.hpp"
#include "Loxi.hpp"
#include "ASTPrinter.hpp"

bool Loxi::hadError = false;
bool Loxi::hadRuntimeError = false;
Interpreter Loxi::interpreter = Interpreter();


void Loxi::run(std::string source) {
    std::unique_ptr<Scanner> scanner = std::make_unique<Scanner>(source);
    std::vector<std::unique_ptr<Token>> tokens = scanner->scanTokens();

    Parser parser (std::move(tokens));
    std::vector<std::unique_ptr<Stmt>> statements = std::move(parser.parse());

    if (hadError) return;
    
    interpreter.interpret(std::move(statements));
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

void Loxi::runtimeError(RuntimeError error){
    std::cout << error.what() << "\n[line " << error.token.line << "]" << std::endl;
    hadRuntimeError = true;
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

    if (hadError) return 65; // Non-zero exit code for error
    if (hadRuntimeError) return 70; // Non-zero exit code for error (different for different type of error)
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
