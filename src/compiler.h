//Implemented bytecode compiler
#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "ast.h"
#include "virtualmachine.h"

class Compiler {
private:
    std::vector<int> bytecode;
    std::map<std::string, int> memoryMap; 
    int nextSlot = 0; 

    static const char* opcodeToString(int opcode) {
        switch (opcode) {
            case OP_PUSH: return "OP_PUSH";
            case OP_ADD: return "OP_ADD";
            case OP_SUB: return "OP_SUB";
            case OP_MUL: return "OP_MUL";
            case OP_DIV: return "OP_DIV";
            case OP_LESS: return "OP_LESS";
            case OP_GREATER: return "OP_GREATER";
            case OP_LESS_EQUAL: return "OP_LESS_EQUAL";
            case OP_GREATER_EQUAL: return "OP_GREATER_EQUAL";
            case OP_EQUAL: return "OP_EQUAL";
            case OP_NEQ: return "OP_NEQ";
            case OP_PRINT: return "OP_PRINT";
            case OP_HALT: return "OP_HALT";
            case OP_SET_VAR: return "OP_SET_VAR";
            case OP_GET_VAR: return "OP_GET_VAR";
            case OP_INPUT: return "OP_INPUT";
            case OP_JUMP_IF_FALSE: return "OP_JUMP_IF_FALSE";
            case OP_JUMP: return "OP_JUMP";
            default: return nullptr;
        }
    }

    static int operandCount(int opcode) {
        switch (opcode) {
            case OP_PUSH:
            case OP_SET_VAR:
            case OP_GET_VAR:
            case OP_INPUT:
            case OP_JUMP_IF_FALSE:
            case OP_JUMP:
                return 1;
            default:
                return 0;
        }
    }

    int getSlot(std::string name) {
        if (memoryMap.find(name) == memoryMap.end()) { memoryMap[name] = nextSlot++; }
        return memoryMap[name];
    }

public:
    void compileNode(std::shared_ptr<ASTNode> node) {
        if (!node) return;

        if (auto numNode = std::dynamic_pointer_cast<NumberNode>(node)) {
            bytecode.push_back(OP_PUSH); bytecode.push_back(numNode->value);
        }
       
        else if (auto boolNode = std::dynamic_pointer_cast<BoolNode>(node)) {
            bytecode.push_back(OP_PUSH); bytecode.push_back(boolNode->value ? 1 : 0);
        }
        else if (auto declNode = std::dynamic_pointer_cast<VarDeclNode>(node)) {
            compileNode(declNode->value); 
            bytecode.push_back(OP_SET_VAR); bytecode.push_back(getSlot(declNode->varName)); 
        }
        else if (auto varNode = std::dynamic_pointer_cast<VariableNode>(node)) {
            bytecode.push_back(OP_GET_VAR); bytecode.push_back(getSlot(varNode->varName)); 
        }
        else if (auto assignNode = std::dynamic_pointer_cast<AssignNode>(node)) {
            compileNode(assignNode->value); 
            bytecode.push_back(OP_SET_VAR); bytecode.push_back(getSlot(assignNode->varName));
        }
        else if (auto blockNode = std::dynamic_pointer_cast<BlockNode>(node)) {
            for (auto stmt : blockNode->statements) compileNode(stmt);
        }
        else if (auto opNode = std::dynamic_pointer_cast<BinaryOpNode>(node)) {
            compileNode(opNode->left);  
            compileNode(opNode->right); 
            switch (opNode->op) {
                case PLUS: bytecode.push_back(OP_ADD); break;
                case MINUS: bytecode.push_back(OP_SUB); break;
                case STAR: bytecode.push_back(OP_MUL); break;
                case SLASH: bytecode.push_back(OP_DIV); break;
                case LESS: bytecode.push_back(OP_LESS); break;
                case GREATER: bytecode.push_back(OP_GREATER); break; 
                case LESS_EQUAL: bytecode.push_back(OP_LESS_EQUAL); break;      
                case GREATER_EQUAL: bytecode.push_back(OP_GREATER_EQUAL); break; 
                case EQUAL_EQUAL: bytecode.push_back(OP_EQUAL); break;
                case BANG_EQUAL: bytecode.push_back(OP_NEQ); break; 
                default: break;
            }
        }
       
        else if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(node)) {
            int loopStart = bytecode.size(); 
            
            compileNode(whileNode->condition); 
            
            bytecode.push_back(OP_JUMP_IF_FALSE); 
            bytecode.push_back(0); 
            int exitJumpPlaceholder = bytecode.size() - 1; 

            compileNode(whileNode->body); 

            bytecode.push_back(OP_JUMP); 
            bytecode.push_back(loopStart);

            bytecode[exitJumpPlaceholder] = bytecode.size();
        }
        
        else if (auto printNode = std::dynamic_pointer_cast<PrintNode>(node)) {
            compileNode(printNode->value); 
            bytecode.push_back(OP_PRINT);  
        }
        else if (auto inputNode = std::dynamic_pointer_cast<InputNode>(node)) {
            bytecode.push_back(OP_INPUT);
            bytecode.push_back(getSlot(inputNode->varName));
        }
        else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(node)) {
            compileNode(ifNode->condition); 
            
            bytecode.push_back(OP_JUMP_IF_FALSE);
            bytecode.push_back(0);
            int thenJumpPlaceholder = bytecode.size() - 1;

            compileNode(ifNode->thenBranch); 

            bytecode.push_back(OP_JUMP);
            bytecode.push_back(0);
            int elseJumpPlaceholder = bytecode.size() - 1;

            bytecode[thenJumpPlaceholder] = bytecode.size(); 
            
            if (ifNode->elseBranch) {
                compileNode(ifNode->elseBranch); 
            }
            
            bytecode[elseJumpPlaceholder] = bytecode.size(); 
        }
    }

    std::vector<int> compile(std::vector<std::shared_ptr<ASTNode>> program) {
        for (auto line : program) compileNode(line); 
        
        bytecode.push_back(OP_HALT);
        return bytecode;
    }

    void printBytecode() const {
        for (std::size_t i = 0; i < bytecode.size();) {
            const int instruction = bytecode[i];
            const char* opcodeName = opcodeToString(instruction);

            if (!opcodeName) {
                std::cout << i << ": " << instruction << "\n";
                ++i;
                continue;
            }

            std::cout << i << ": " << opcodeName;

            const int operands = operandCount(instruction);
            for (int operandIndex = 0; operandIndex < operands; ++operandIndex) {
                const std::size_t operandPosition = i + 1 + operandIndex;
                if (operandPosition < bytecode.size()) {
                    std::cout << " " << bytecode[operandPosition];
                } else {
                    std::cout << " <missing>";
                }
            }

            std::cout << "\n";
            i += 1 + operands;
        }
    }
};
