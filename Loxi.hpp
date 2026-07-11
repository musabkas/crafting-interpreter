#pragma once
#include <string>
#include "RuntimeError.hpp"
#include "Interpreter.hpp"

class Loxi {
    static bool hadError;
    static bool hadRuntimeError;
    static Interpreter interpreter;
    bool needsContinuation(const std::string& totalInput);
    
    protected:
    void run(std::string source);
    static void report(int line, std::string where, std::string message);
    
    public:
    Loxi() = default;
    static void error(int line, std::string message);
    static void error(Token token, std::string message);
    static void runtimeError(RuntimeError error);
    void runPrompt();
    int runScript(std::string path);
};