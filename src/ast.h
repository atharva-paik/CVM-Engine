#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>
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

struct UnaryOpNode : public ASTNode {
    TokenType op;
    std::shared_ptr<ASTNode> operand;
    UnaryOpNode(TokenType operation, std::shared_ptr<ASTNode> expr) {
        op = operation;
        operand = expr;
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

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case PLUS: return "+";
        case MINUS: return "-";
        case STAR: return "*";
        case SLASH: return "/";
        case EQUAL_EQUAL: return "==";
        case BANG_EQUAL: return "!=";
        case LESS: return "<";
        case GREATER: return ">";
        case LESS_EQUAL: return "<=";
        case GREATER_EQUAL: return ">=";
        default: return "?";
    }
}

inline void printIndent(int indent) {
    for (int i = 0; i < indent; ++i) std::cout << "  ";
}

inline void printASTNode(const std::shared_ptr<ASTNode>& node, int indent = 0) {
    if (!node) {
        printIndent(indent);
        std::cout << "<null>\n";
        return;
    }

    if (auto numberNode = std::dynamic_pointer_cast<NumberNode>(node)) {
        printIndent(indent);
        std::cout << "Number(" << numberNode->value << ")\n";
    } else if (auto boolNode = std::dynamic_pointer_cast<BoolNode>(node)) {
        printIndent(indent);
        std::cout << "Bool(" << (boolNode->value ? "true" : "false") << ")\n";
    } else if (auto variableNode = std::dynamic_pointer_cast<VariableNode>(node)) {
        printIndent(indent);
        std::cout << "Variable(" << variableNode->varName << ")\n";
    } else if (auto unaryNode = std::dynamic_pointer_cast<UnaryOpNode>(node)) {
        printIndent(indent);
        std::cout << "UnaryOp(" << tokenTypeToString(unaryNode->op) << ")\n";
        printASTNode(unaryNode->operand, indent + 1);
    } else if (auto binaryNode = std::dynamic_pointer_cast<BinaryOpNode>(node)) {
        printIndent(indent);
        std::cout << "BinaryOp(" << tokenTypeToString(binaryNode->op) << ")\n";
        printASTNode(binaryNode->left, indent + 1);
        printASTNode(binaryNode->right, indent + 1);
    } else if (auto varDeclNode = std::dynamic_pointer_cast<VarDeclNode>(node)) {
        printIndent(indent);
        std::cout << "VarDecl(" << varDeclNode->varName << ")\n";
        printASTNode(varDeclNode->value, indent + 1);
    } else if (auto assignNode = std::dynamic_pointer_cast<AssignNode>(node)) {
        printIndent(indent);
        std::cout << "Assign(" << assignNode->varName << ")\n";
        printASTNode(assignNode->value, indent + 1);
    } else if (auto printNode = std::dynamic_pointer_cast<PrintNode>(node)) {
        printIndent(indent);
        std::cout << "Print\n";
        printASTNode(printNode->value, indent + 1);
    } else if (auto inputNode = std::dynamic_pointer_cast<InputNode>(node)) {
        printIndent(indent);
        std::cout << "Input(" << inputNode->varName << ")\n";
    } else if (auto blockNode = std::dynamic_pointer_cast<BlockNode>(node)) {
        printIndent(indent);
        std::cout << "Block\n";
        for (const auto& statement : blockNode->statements) {
            printASTNode(statement, indent + 1);
        }
    } else if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(node)) {
        printIndent(indent);
        std::cout << "While\n";
        printIndent(indent + 1);
        std::cout << "Condition\n";
        printASTNode(whileNode->condition, indent + 2);
        printIndent(indent + 1);
        std::cout << "Body\n";
        printASTNode(whileNode->body, indent + 2);
    } else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(node)) {
        printIndent(indent);
        std::cout << "If\n";
        printIndent(indent + 1);
        std::cout << "Condition\n";
        printASTNode(ifNode->condition, indent + 2);
        printIndent(indent + 1);
        std::cout << "Then\n";
        printASTNode(ifNode->thenBranch, indent + 2);
        if (ifNode->elseBranch) {
            printIndent(indent + 1);
            std::cout << "Else\n";
            printASTNode(ifNode->elseBranch, indent + 2);
        }
    } else {
        printIndent(indent);
        std::cout << "UnknownNode\n";
    }
}

inline void printAST(const std::vector<std::shared_ptr<ASTNode>>& program) {
    std::cout << "===== AST =====\n";
    for (const auto& statement : program) {
        printASTNode(statement, 0);
    }
    std::cout << "===============\n";
}
