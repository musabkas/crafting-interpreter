#include <string>
#include <vector>

class Token{

};

class Scanner{
private:
    
public:
    Scanner(std::string source);
    std::vector<Token> scanTokens();
};