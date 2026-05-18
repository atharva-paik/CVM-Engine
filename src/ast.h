//Implemented Abstract Syntax Tree logic
#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "lexer.h"

inline const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case STAR: return "STAR";
        case SLASH: return "SLASH";
        case ASSIGN: return "ASSIGN";
        case EQUAL_EQUAL: return "EQUAL_EQUAL";
        case LESS: return "LESS";
        case GREATER: return "GREATER";
        case LESS_EQUAL: return "LESS_EQUAL";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case BOOL_TRUE: return "BOOL_TRUE";
        case BOOL_FALSE: return "BOOL_FALSE";
        case BANG_EQUAL: return "BANG_EQUAL";
        case LEFT_PAREN: return "LEFT_PAREN";
        case RIGHT_PAREN: return "RIGHT_PAREN";
        case LEFT_BRACE: return "LEFT_BRACE";
        case RIGHT_BRACE: return "RIGHT_BRACE";
        case SEMICOLON: return "SEMICOLON";
        case NUMBER: return "NUMBER";
        case IDENTIFIER: return "IDENTIFIER";
        case LET: return "LET";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case PRINT: return "PRINT";
        case INPUT: return "INPUT";
        case EOF_TOKEN: return "EOF_TOKEN";
        default: return "UNKNOWN_TOKEN";
    }
}

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print(int depth = 0) const = 0;
};

struct NumberNode : public ASTNode {
    int value;
    NumberNode(int val) { value = val; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "NumberNode: " << value << "\n";
    }
};

struct BinaryOpNode : public ASTNode {
    TokenType op; 
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    BinaryOpNode(TokenType operation, std::shared_ptr<ASTNode> leftChild, std::shared_ptr<ASTNode> rightChild) {
        op = operation; left = leftChild; right = rightChild;
    }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "BinaryOpNode: " << tokenTypeToString(op) << "\n";
        if (left) left->print(depth + 1);
        if (right) right->print(depth + 1);
    }
};

struct VarDeclNode : public ASTNode {
    std::string varName;
    std::shared_ptr<ASTNode> value; 
    VarDeclNode(std::string name, std::shared_ptr<ASTNode> val) { varName = name; value = val; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "VarDeclNode: " << varName << "\n";
        if (value) value->print(depth + 1);
    }
};

struct VariableNode : public ASTNode {
    std::string varName;
    VariableNode(std::string name) { varName = name; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "VariableNode: " << varName << "\n";
    }
};


struct AssignNode : public ASTNode {
    std::string varName;
    std::shared_ptr<ASTNode> value;
    AssignNode(std::string name, std::shared_ptr<ASTNode> val) { varName = name; value = val; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "AssignNode: " << varName << "\n";
        if (value) value->print(depth + 1);
    }
};

struct BlockNode : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> statements;
    BlockNode(std::vector<std::shared_ptr<ASTNode>> stmts) { statements = stmts; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "BlockNode\n";
        for (const auto& statement : statements) {
            if (statement) statement->print(depth + 1);
        }
    }
};

struct WhileNode : public ASTNode {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
    WhileNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> b) { condition = cond; body = b; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "WhileNode\n";
        if (condition) condition->print(depth + 1);
        if (body) body->print(depth + 1);
    }
};


struct PrintNode : public ASTNode {
    std::shared_ptr<ASTNode> value;
    PrintNode(std::shared_ptr<ASTNode> val) { value = val; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "PrintNode\n";
        if (value) value->print(depth + 1);
    }
};

struct InputNode : public ASTNode {
    std::string varName;
    InputNode(std::string name) { varName = name; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "InputNode: " << varName << "\n";
    }
};

struct IfNode : public ASTNode {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> thenBranch;
    std::shared_ptr<ASTNode> elseBranch; 
    
    IfNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> thenB, std::shared_ptr<ASTNode> elseB) {
        condition = cond; thenBranch = thenB; elseBranch = elseB;
    }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "IfNode\n";
        if (condition) condition->print(depth + 1);
        if (thenBranch) thenBranch->print(depth + 1);
        if (elseBranch) elseBranch->print(depth + 1);
    }
};

struct BoolNode : public ASTNode {
    bool value;
    BoolNode(bool val) { value = val; }

    void print(int depth = 0) const override {
        std::string indent(depth * 2, ' ');
        std::cout << indent << "BoolNode: " << (value ? "true" : "false") << "\n";
    }
};
