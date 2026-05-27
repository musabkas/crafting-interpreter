#include <iostream>
#include <string>

int main(int argc, char* argv[]){
    if (argc > 2) {
        std::cout << "Usage: loxi [script]" << std::endl;
    } else if (argc == 2) {
        std::cout << "running script " << argv[1] << std::endl;
        // runFile(argv[0]);
    } else {
        std::cout << "running prompt " << std::endl;
        // runPrompt();
    }
}