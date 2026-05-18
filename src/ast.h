//Implemented Abstract Syntax Tree logic
#pragma once
#include <string>
#include <memory>
#include <vector>
#include "lexer.h"

struct ASTNode { virtual ~ASTNode() = default; };

struct NumberNode : public ASTNode {
    int value;
    NumberNode(int val) { value = val; }
};

struct BinaryOpNode : public ASTNode {
    TokenType op; 
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    BinaryOpNode(TokenType operation, std::shared_ptr<ASTNode> leftChild, std::shared_ptr<ASTNode> rightChild) {
        op = operation; left = leftChild; right = rightChild;
    }
};

struct VarDeclNode : public ASTNode {
    std::string varName;
    std::shared_ptr<ASTNode> value; 
    VarDeclNode(std::string name, std::shared_ptr<ASTNode> val) { varName = name; value = val; }
};

struct VariableNode : public ASTNode {
    std::string varName;
    VariableNode(std::string name) { varName = name; }
};


struct AssignNode : public ASTNode {
    std::string varName;
    std::shared_ptr<ASTNode> value;
    AssignNode(std::string name, std::shared_ptr<ASTNode> val) { varName = name; value = val; }
};

struct BlockNode : public ASTNode {
    std::vector<std::shared_ptr<ASTNode>> statements;
    BlockNode(std::vector<std::shared_ptr<ASTNode>> stmts) { statements = stmts; }
};

struct WhileNode : public ASTNode {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
    WhileNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> b) { condition = cond; body = b; }
};


struct PrintNode : public ASTNode {
    std::shared_ptr<ASTNode> value;
    PrintNode(std::shared_ptr<ASTNode> val) { value = val; }
};

struct InputNode : public ASTNode {
    std::string varName;
    InputNode(std::string name) { varName = name; }
};

struct IfNode : public ASTNode {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> thenBranch;
    std::shared_ptr<ASTNode> elseBranch; 
    
    IfNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> thenB, std::shared_ptr<ASTNode> elseB) {
        condition = cond; thenBranch = thenB; elseBranch = elseB;
    }
};
struct BoolNode : public ASTNode {
    bool value;
    BoolNode(bool val) { value = val; }
};
