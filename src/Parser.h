#pragma once
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include "lexer.h"
#include "ast.h"

class Parser {
private:
    std::vector<Token> tokens;
    int current = 0;

    Token peek() { return tokens[current]; }
    bool isAtEnd() { return peek().type == EOF_TOKEN; }
    bool match(TokenType type) {
        if (peek().type == type) { current++; return true; }
        return false;
    }

    Token consume(TokenType type, const std::string& message) {
        if (peek().type == type) {
            current++;
            return tokens[current - 1];
        }
        throw std::runtime_error("Parse Error: " + message);
    }

public:
    Parser(std::vector<Token> tokenList) { tokens = tokenList; }

    std::shared_ptr<ASTNode> primary() {
        if (match(NUMBER)) return std::make_shared<NumberNode>(std::stoi(tokens[current - 1].lexeme));
        if (match(IDENTIFIER)) return std::make_shared<VariableNode>(tokens[current - 1].lexeme);
        if (match(BOOL_TRUE)) return std::make_shared<BoolNode>(true);
        if (match(BOOL_FALSE)) return std::make_shared<BoolNode>(false);
        
       
        if (match(LEFT_PAREN)) {
            auto expr = comparison(); 
            consume(RIGHT_PAREN, "Expected ')' after expression.");
            return expr;
        }
        throw std::runtime_error("Parse Error: Expected expression.");
    }

    std::shared_ptr<ASTNode> unary() {
        if (match(MINUS)) {
            return std::make_shared<UnaryOpNode>(MINUS, unary());
        }
        return primary();
    }

    std::shared_ptr<ASTNode> factor() {
        std::shared_ptr<ASTNode> left = unary();
        while (match(STAR) || match(SLASH)) {
            TokenType op = tokens[current - 1].type;
            left = std::make_shared<BinaryOpNode>(op, left, unary());
        }
        return left;
    }

    std::shared_ptr<ASTNode> term() {
        std::shared_ptr<ASTNode> left = factor();
        while (match(PLUS) || match(MINUS)) {
            TokenType op = tokens[current - 1].type;
            left = std::make_shared<BinaryOpNode>(op, left, factor());
        }
        return left;
    }

    std::shared_ptr<ASTNode> comparison() {
        std::shared_ptr<ASTNode> left = term();
       
        while (match(EQUAL_EQUAL) || match(BANG_EQUAL) || match(LESS) || match(GREATER) || match(LESS_EQUAL) || match(GREATER_EQUAL)) {
            TokenType op = tokens[current - 1].type;
            left = std::make_shared<BinaryOpNode>(op, left, term());
        }
        return left;
    }

    std::shared_ptr<ASTNode> statement() {
       
        while (match(SEMICOLON)) {} 

        if (match(LET)) {
            Token name = consume(IDENTIFIER, "Expected variable name after 'let'.");
            std::string varName = name.lexeme;
            consume(ASSIGN, "Expected '=' after variable name.");
            auto decl = std::make_shared<VarDeclNode>(varName, comparison());
            consume(SEMICOLON, "Expected ';' after variable declaration.");
            return decl;
        }
        
        if (match(PRINT)) {
            auto val = comparison();
            consume(SEMICOLON, "Expected ';' after print statement.");
            return std::make_shared<PrintNode>(val);
        }

        if (match(INPUT)) {
            Token name = consume(IDENTIFIER, "Expected variable name after 'input'.");
            std::string varName = name.lexeme;
            consume(SEMICOLON, "Expected ';' after input statement.");
            return std::make_shared<InputNode>(varName);
        }

        if (match(WHILE)) {
            consume(LEFT_PAREN, "Expected '(' after 'while'.");
            std::shared_ptr<ASTNode> condition = comparison(); 
            consume(RIGHT_PAREN, "Expected ')' after while condition.");
            
            consume(LEFT_BRACE, "Expected '{' before while body.");
            std::vector<std::shared_ptr<ASTNode>> bodyStmts;
            while (!isAtEnd() && peek().type != RIGHT_BRACE) {
                bodyStmts.push_back(statement()); 
            }
            consume(RIGHT_BRACE, "Expected '}' after while body.");
            return std::make_shared<WhileNode>(condition, std::make_shared<BlockNode>(bodyStmts));
        }

        if (match(IF)) {
            consume(LEFT_PAREN, "Expected '(' after 'if'.");
            std::shared_ptr<ASTNode> condition = comparison();
            consume(RIGHT_PAREN, "Expected ')' after if condition.");
            
            consume(LEFT_BRACE, "Expected '{' before if body.");
            std::vector<std::shared_ptr<ASTNode>> thenStmts;
            while (!isAtEnd() && peek().type != RIGHT_BRACE) {
                thenStmts.push_back(statement());
            }
            consume(RIGHT_BRACE, "Expected '}' after if body.");
            auto thenBranch = std::make_shared<BlockNode>(thenStmts);

            std::shared_ptr<ASTNode> elseBranch = nullptr;
            if (match(ELSE)) {
                consume(LEFT_BRACE, "Expected '{' before else body.");
                std::vector<std::shared_ptr<ASTNode>> elseStmts;
                while (!isAtEnd() && peek().type != RIGHT_BRACE) {
                    elseStmts.push_back(statement());
                }
                consume(RIGHT_BRACE, "Expected '}' after else body.");
                elseBranch = std::make_shared<BlockNode>(elseStmts);
            }
            return std::make_shared<IfNode>(condition, thenBranch, elseBranch);
        }

        if (peek().type == IDENTIFIER && current + 1 < tokens.size() && tokens[current + 1].type == ASSIGN) {
            Token name = consume(IDENTIFIER, "Expected variable name.");
            std::string varName = name.lexeme;
            consume(ASSIGN, "Expected '=' in assignment.");
            auto assign = std::make_shared<AssignNode>(varName, comparison());
            consume(SEMICOLON, "Expected ';' after assignment.");
            return assign;
        }
        
        auto expr = comparison();
        consume(SEMICOLON, "Expected ';' after expression.");
        
       
        return std::make_shared<PrintNode>(expr);
    }

    std::vector<std::shared_ptr<ASTNode>> parse() {
        std::vector<std::shared_ptr<ASTNode>> program;
        while (!isAtEnd()) {
            program.push_back(statement());
        }
        return program;
    }
};
