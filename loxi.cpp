#include <iostream>
#include <fstream>
#include <string>

#include "Scanner.hpp"

void run(std::string source) {
    std::shared_ptr<Scanner> scanner = std::make_shared<Scanner>(source);
    std::vector<Token> tokens = scanner->scanTokens();

    for (Token token : tokens) {
        // std::cout << token << std::endl;
    }
}

void runPrompt() {
    while (true) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) break; // if EOF sent then end program, else read line
        run(line);
    }
    std::cout << std::endl;
}

void runScript(std::string path){
    std::ifstream script(path);
    if (!script.is_open()) {
        std::cout << "Could not open file: " << path << std::endl;
        return;
    }

    std::string source;
    std::string line;
    while (std::getline(script, line)) {
        source += line + "\n";
    }
    source.pop_back(); // remove final \n
    run(source);
}


int main(int argc, char* argv[]){
    if (argc > 2) {
        std::cout << "Usage: loxi [script]" << std::endl;
    } else if (argc == 2) {
        std::cout << "Running script " << argv[1] << std::endl;
        runScript(argv[1]);
    } else {
        std::cout << "Running prompt " << std::endl;
        runPrompt();
    }
}