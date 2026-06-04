#pragma once
#include <string>

class Loxi {
    static bool hadError;
    
    protected:
    void run(std::string source);
    static void report(int line, std::string where, std::string message);
    
    public:
    Loxi() = default;
    static void error(int line, std::string message);
    static void error(Token token, std::string message);
    void runPrompt();
    int runScript(std::string path);
};