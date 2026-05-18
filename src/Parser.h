#pragma once
#include <vector>
#include <memory>
#include <string>
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

public:
    Parser(std::vector<Token> tokenList) { tokens = tokenList; }

    std::shared_ptr<ASTNode> primary() {
        if (match(NUMBER)) return std::make_shared<NumberNode>(std::stoi(tokens[current - 1].lexeme));
        if (match(IDENTIFIER)) return std::make_shared<VariableNode>(tokens[current - 1].lexeme);
        if (match(BOOL_TRUE)) return std::make_shared<BoolNode>(true);
        if (match(BOOL_FALSE)) return std::make_shared<BoolNode>(false);
        
       
        if (match(LEFT_PAREN)) {
            auto expr = comparison(); 
            match(RIGHT_PAREN);
            return expr;
        }
        return nullptr; 
    }

    std::shared_ptr<ASTNode> factor() {
        std::shared_ptr<ASTNode> left = primary();
        while (match(STAR) || match(SLASH)) {
            TokenType op = tokens[current - 1].type;
            left = std::make_shared<BinaryOpNode>(op, left, primary());
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
            match(IDENTIFIER); std::string varName = tokens[current - 1].lexeme;
            match(ASSIGN);
            auto decl = std::make_shared<VarDeclNode>(varName, comparison());
            match(SEMICOLON); 
            return decl;
        }
        
        if (match(PRINT)) {
            auto val = comparison();
            match(SEMICOLON);
            return std::make_shared<PrintNode>(val);
        }

        if (match(INPUT)) {
            match(IDENTIFIER);
            std::string varName = tokens[current - 1].lexeme;
            match(SEMICOLON);
            return std::make_shared<InputNode>(varName);
        }

        if (match(WHILE)) {
            match(LEFT_PAREN); 
            std::shared_ptr<ASTNode> condition = comparison(); 
            match(RIGHT_PAREN); 
            
            match(LEFT_BRACE); 
            std::vector<std::shared_ptr<ASTNode>> bodyStmts;
            while (!isAtEnd() && peek().type != RIGHT_BRACE) {
                bodyStmts.push_back(statement()); 
            }
            match(RIGHT_BRACE); 
            return std::make_shared<WhileNode>(condition, std::make_shared<BlockNode>(bodyStmts));
        }

        if (match(IF)) {
            match(LEFT_PAREN);
            std::shared_ptr<ASTNode> condition = comparison();
            match(RIGHT_PAREN);
            
            match(LEFT_BRACE);
            std::vector<std::shared_ptr<ASTNode>> thenStmts;
            while (!isAtEnd() && peek().type != RIGHT_BRACE) {
                thenStmts.push_back(statement());
            }
            match(RIGHT_BRACE);
            auto thenBranch = std::make_shared<BlockNode>(thenStmts);

            std::shared_ptr<ASTNode> elseBranch = nullptr;
            if (match(ELSE)) {
                match(LEFT_BRACE);
                std::vector<std::shared_ptr<ASTNode>> elseStmts;
                while (!isAtEnd() && peek().type != RIGHT_BRACE) {
                    elseStmts.push_back(statement());
                }
                match(RIGHT_BRACE);
                elseBranch = std::make_shared<BlockNode>(elseStmts);
            }
            return std::make_shared<IfNode>(condition, thenBranch, elseBranch);
        }

        if (peek().type == IDENTIFIER && current + 1 < tokens.size() && tokens[current + 1].type == ASSIGN) {
            match(IDENTIFIER); std::string varName = tokens[current - 1].lexeme;
            match(ASSIGN);
            auto assign = std::make_shared<AssignNode>(varName, comparison());
            match(SEMICOLON);
            return assign;
        }
        
        auto expr = comparison();
        match(SEMICOLON); 
        
       
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