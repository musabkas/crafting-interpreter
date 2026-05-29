#include <string>

class Loxi {
    bool hadError;
    
    protected:
    void run(std::string source);
    
    public:
    Loxi();
    void runPrompt();
    void runScript(std::string path);
};