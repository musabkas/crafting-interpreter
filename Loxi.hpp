#include <string>

class Loxi {
    bool hadError;
    
    protected:
    void run(std::string source);
    void error(int line, std::string message);
    void report(int line, std::string where, std::string message);

    public:
    Loxi();
    void runPrompt();
    int runScript(std::string path);
};