#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
enum Opcode {
    OP_PUSH, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_LESS, OP_GREATER, 
    OP_LESS_EQUAL, OP_GREATER_EQUAL, 
    OP_EQUAL, OP_NEQ, OP_PRINT, OP_HALT,
    OP_SET_VAR, OP_GET_VAR, OP_INPUT, OP_JUMP_IF_FALSE, 
    OP_JUMP          
};

inline std::string opcodeName(int opcode) {
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
        default: return "UNKNOWN";
    }
}

inline bool opcodeHasOperand(int opcode) {
    return opcode == OP_PUSH ||
           opcode == OP_SET_VAR ||
           opcode == OP_GET_VAR ||
           opcode == OP_INPUT ||
           opcode == OP_JUMP_IF_FALSE ||
           opcode == OP_JUMP;
}

inline void printBytecode(const std::vector<int>& bytecode) {
    std::cout << "===== BYTECODE =====\n";
    for (size_t i = 0; i < bytecode.size(); ++i) {
        int opcode = bytecode[i];
        std::cout << i << ": " << opcodeName(opcode);
        if (opcodeHasOperand(opcode) && i + 1 < bytecode.size()) {
            std::cout << " " << bytecode[i + 1];
            ++i;
        }
        std::cout << "\n";
    }
    std::cout << "====================\n";
}

class VM {
private:
    std::vector<int> stack; 
    std::vector<int> code;  
    int ip = 0;             
    int memory[256] = {0}; 

    void push(int value) { stack.push_back(value); }
    
  
    int pop() {
        if (stack.empty()) {
            throw std::runtime_error("CRITICAL ERROR: Stack Underflow (Tried to pop an empty stack!)");
        }
        int value = stack.back();
        stack.pop_back();
        return value;
    }

public:
    void load(std::vector<int> bytecode) { code = bytecode; ip = 0; }

    void run() {
        while (ip < code.size()) {
            int instruction = code[ip];

            if (instruction == OP_HALT) { break; }
            else if (instruction == OP_INPUT) {
                ip++;
                int slot = code[ip]; 
                int userInput;
                std::cin >> userInput;
                memory[slot] = userInput;
                ip++;
            }
            else if (instruction == OP_PUSH) { ip++; push(code[ip]); ip++; }
            else if (instruction == OP_ADD) { int b = pop(); int a = pop(); push(a + b); ip++; }
            else if (instruction == OP_SUB) { int b = pop(); int a = pop(); push(a - b); ip++; }
            else if (instruction == OP_MUL) { int b = pop(); int a = pop(); push(a * b); ip++; }
            else if (instruction == OP_DIV) { int b = pop(); int a = pop(); push(a / b); ip++; }
            else if (instruction == OP_LESS) { int b = pop(); int a = pop(); push(a < b ? 1 : 0); ip++; }
            else if (instruction == OP_GREATER) { int b = pop(); int a = pop(); push(a > b ? 1 : 0); ip++; } 
            else if (instruction == OP_LESS_EQUAL) { int b = pop(); int a = pop(); push(a <= b ? 1 : 0); ip++; }    
            else if (instruction == OP_GREATER_EQUAL) { int b = pop(); int a = pop(); push(a >= b ? 1 : 0); ip++; }
            else if (instruction == OP_EQUAL) { int b = pop(); int a = pop(); push(a == b ? 1 : 0); ip++; }
            else if (instruction == OP_NEQ) { int b = pop(); int a = pop(); push(a != b ? 1 : 0); ip++; }   
            
            else if (instruction == OP_SET_VAR) {
                ip++;
                int slot = code[ip];  
                memory[slot] = pop(); 
                ip++;
            }
            else if (instruction == OP_GET_VAR) {
                ip++;
                int slot = code[ip]; 
                push(memory[slot]);  
                ip++;
            }
            
           
            else if (instruction == OP_JUMP_IF_FALSE) {
                ip++;
                int targetIndex = code[ip]; 
                if (pop() == 0) { 
                    ip = targetIndex; 
                } else {
                    ip++; 
                }
            }
            else if (instruction == OP_JUMP) {
                ip++;
                int targetIndex = code[ip]; 
                ip = targetIndex;
            }
            

            else if (instruction == OP_PRINT) { std::cout << "Your output: " << pop() << "\n"; ip++; }
            else { std::cout << "VM ERROR: Unknown instruction!\n"; break; }
        }
    }
};
