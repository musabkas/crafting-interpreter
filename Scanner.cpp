#include "Scanner.hpp"
#include "Loxi.hpp"


std::map<std::string, TokenType> Scanner::keywords = {
    {"and", AND},
    {"class", CLASS},
    {"else", ELSE},
    {"false", FALSE},
    {"for", FOR},
    {"fun", FUN},
    {"if", IF},
    {"nil", NIL},
    {"or", OR},
    {"print", PRINT},
    {"return", RETURN},
    {"super", SUPER},
    {"this", THIS},
    {"true", TRUE},
    {"var", VAR},
    {"while", WHILE}
};

Scanner::Scanner(std::string source) {
    this->source = source;
    this->tokens = {};
    this->start = 0;
    this->current = 0;
    this->line = 1;
}

bool Scanner::isAtEnd() {
    return current >= source.size();
}

void Scanner::scanToken(){
    char c = advance();
    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
        case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
        case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
        case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
        case '/': 
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            // ignore white space
            break;
        case '\n':
            line++;
            break;
        case '"': string(); break;
        default: 
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                Loxi::error(line, "Unexpected character.");
            }
            break;
    }
}

char Scanner::advance(){
    return source.at(current++);
}

char Scanner::peek(){
    if (isAtEnd()) return '\0';
    return source.at(current);
}

char Scanner::peekNext(){
    if (current + 1 >= source.size()) return '\0';
    return source.at(current + 1);
}

bool Scanner::match(char expected){
    if (isAtEnd()) return false;
    if (source.at(current) != expected) return false;

    current++;
    return true;
}

bool Scanner::isDigit(char c) {
    return '0' <= c && c <= '9';
}

bool Scanner::isAlpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Scanner::string(){
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) {
        Loxi::error(line, "Unterminated string.");
        return;
    }

    advance(); // closing "
    std::string s = source.substr(start+1, current - start - 2);
    addToken(STRING, s);
}

void Scanner::number(){
    while (isDigit(peek())) advance();
    
    if (peek() == '.' && isDigit(peekNext())) {
        advance(); // consume '.'
        while (isDigit(peek()) && !isAtEnd()) advance();
    }

    double num = std::stod(source.substr(start, current - start));
    addToken(NUMBER, num);
}

void Scanner::identifier(){
    while (isAlphaNumeric(peek())) advance();

    std::string kw = source.substr(start, current - start);
    if (keywords.count(kw)) {
        addToken(keywords[kw]);
    } else {
        addToken(IDENTIFIER);
    }
}

void Scanner::addToken(TokenType type){
    addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, std::variant<double, std::string, void *> literal){
    tokens.push_back(new Token(type, source.substr(start, current - start), literal, line));
}

std::vector<Token*> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.push_back(new Token(END_OF_FILE, "", nullptr, line));
    return tokens;
}