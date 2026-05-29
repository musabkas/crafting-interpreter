#include <string>
#include <vector>
#include "Token.hpp"

class Scanner{
private:

public:
    Scanner(std::string source);
    std::vector<Token> scanTokens();
};