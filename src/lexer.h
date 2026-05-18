#pragma once
#include <string>
#include <vector>
#include <cctype>

enum TokenType {
    PLUS, MINUS, STAR, SLASH,
    ASSIGN, EQUAL_EQUAL, LESS, GREATER,
    LESS_EQUAL, GREATER_EQUAL,  
    BOOL_TRUE, BOOL_FALSE, BANG_EQUAL,
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    SEMICOLON,
    NUMBER, IDENTIFIER,
    LET, IF, ELSE, WHILE, PRINT, INPUT,
    EOF_TOKEN 
};

struct Token {
    TokenType type;
    std::string lexeme;
};

class Lexer {
private:
    std::string source;
    int current = 0;
    std::vector<Token> tokens;

    bool isAtEnd() {
        return current >= source.length();
    }

public:
    Lexer(std::string source_code) {
        source = source_code;
    }

    std::vector<Token> tokenize() {
        while (!isAtEnd()) {
            char c = source[current];

           
            if (isspace(c)) {
                current++;
            } 
            
            else if (c == '+') { tokens.push_back({PLUS, "+"}); current++; }
            else if (c == '-') { tokens.push_back({MINUS, "-"}); current++; }
            else if (c == '*') { tokens.push_back({STAR, "*"}); current++; }
            else if (c == '/') {
                if (current + 1 < source.length() && source[current + 1] == '/') {
                    while (!isAtEnd() && source[current] != '\n') {
                        current++;
                    }
                } else {
                    tokens.push_back({SLASH, "/"}); 
                    current++;
                }
            }
            else if (c == '<') {
                if (current + 1 < source.length() && source[current + 1] == '=') {
                    tokens.push_back({LESS_EQUAL, "<="}); current += 2;
                } else {
                    tokens.push_back({LESS, "<"}); current++;
                }
            }
            else if (c == '>') {
                if (current + 1 < source.length() && source[current + 1] == '=') {
                    tokens.push_back({GREATER_EQUAL, ">="}); current += 2;
                } else {
                    tokens.push_back({GREATER, ">"}); current++;
                }
            }
            else if (c == '{') { tokens.push_back({LEFT_BRACE, "{"}); current++; }
            else if (c == '}') { tokens.push_back({RIGHT_BRACE, "}"}); current++; }
            else if (c == '(') { tokens.push_back({LEFT_PAREN, "("}); current++; }
            else if (c == ')') { tokens.push_back({RIGHT_PAREN, ")"}); current++; }
            else if (c == ';') { tokens.push_back({SEMICOLON, ";"}); current++; }
            else if (c == '=') {
                if (current + 1 < source.length() && source[current + 1] == '=') {
                    tokens.push_back({EQUAL_EQUAL, "=="});
                    current += 2; 
                } else {
                    tokens.push_back({ASSIGN, "="});
                    current++;
                }
            }
            else if (c == '!') {
                if (current + 1 < source.length() && source[current + 1] == '=') {
                    tokens.push_back({BANG_EQUAL, "!="});
                    current += 2; 
                } else {
                    current++;
                }
            }
           
            else if (isdigit(c)) {
                std::string lexeme = "";
                while (!isAtEnd() && isdigit(source[current])) {
                    lexeme += source[current];
                    current++;
                }
                tokens.push_back({NUMBER, lexeme});
            }
           
            else if (isalpha(c)) {
                std::string lexeme = "";
                while (!isAtEnd() && (isalpha(source[current]) || isdigit(source[current]))) {
                    lexeme += source[current];
                    current++;
                }
                
                if (lexeme == "let") tokens.push_back({LET, lexeme});
                else if (lexeme == "if") tokens.push_back({IF, lexeme});
                else if (lexeme == "else") tokens.push_back({ELSE, lexeme});   
                else if (lexeme == "true") tokens.push_back({BOOL_TRUE, lexeme});
                else if (lexeme == "false") tokens.push_back({BOOL_FALSE, lexeme});
                else if (lexeme == "while") tokens.push_back({WHILE, lexeme});
                else if (lexeme == "print") tokens.push_back({PRINT, lexeme});
                else if (lexeme == "input") tokens.push_back({INPUT, lexeme}); 
                else tokens.push_back({IDENTIFIER, lexeme});
            }
           
            else {
                current++;
            }
        }
        
        tokens.push_back({EOF_TOKEN, ""});
        return tokens;
    }
};