#include <fstream>
#include <iostream>

#include "Scanner.hpp"
#include "Loxi.hpp"

bool Loxi::hadError = false;

void Loxi::run(std::string source) {
    std::unique_ptr<Scanner> scanner = std::make_unique<Scanner>(source);
    std::vector<std::unique_ptr<Token>> tokens = scanner->scanTokens();

    for (std::unique_ptr<Token>& token : tokens) {
        std::cout << *token << std::endl;
    }
}
void Loxi::error(int line, std::string message){
    report(line, "", message);
}

void Loxi::report(int line, std::string where, std::string message){
    std::cout << "[line " << line << "] Error" + where + ": " + message << std::endl;
    hadError = true;
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
