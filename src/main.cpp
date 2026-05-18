#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "lexer.h" 
#include "ast.h"
#include "Parser.h" 
#include "compiler.h" 
#include "virtualmachine.h" 


void runPipeline(std::string code, bool showAST, bool showBytecode, long long stepLimit) {
    try {
        Lexer lexer(code);
        std::vector<Token> tokens = lexer.tokenize();
        Parser parser(tokens);
        Compiler compiler;
        VM vm;
        
        std::vector<std::shared_ptr<ASTNode>> program = parser.parse();
        if (showAST) {
            printAST(program);
        }

        std::vector<int> bytecode = compiler.compile(program);
        if (showBytecode) {
            printBytecode(bytecode);
        }
        
        std::cout << " -> ";
        vm.load(bytecode);
        vm.setStepLimit(stepLimit);
        vm.run(); 
        std::cout << "==========================\n";
        
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    } catch (...) {
        std::cout << "Syntax Error: Unrecognized command or structure.\n";
    }
}

int main(int argc, char* argv[]) {
    bool showAST = false;
    bool showBytecode = false;
    long long stepLimit = 100000;
    std::string filename = "";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ast" || arg == "-ast") {
            showAST = true;
        } else if (arg == "--bytecode" || arg == "-bytecode") {
            showBytecode = true;
        } else if (arg == "--debug" || arg == "-debug") {
            showAST = true;
            showBytecode = true;
        } else if ((arg == "--max-steps" || arg == "-max-steps") && i + 1 < argc) {
            stepLimit = std::atoll(argv[++i]);
            if (stepLimit <= 0) {
                stepLimit = 100000;
            }
        } else if (filename.empty()) {
            filename = arg;
        } else {
            std::cout << "Warning: Ignoring extra argument '" << arg << "'\n";
        }
    }

    if (!filename.empty()) {
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "Error: Could not open file '" << filename << "'\n";
            return 1;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf(); 
        
        std::cout << "Running " << filename << "...\n";
        std::cout << "-----------------------------------------\n";
        runPipeline(buffer.str(), showAST, showBytecode, stepLimit);
        std::cout << "-----------------------------------------\n";
        return 0;
    }

   
    std::cout << "-----------------------------------------\n";
    std::cout << "        CVM++ INTERACTIVE TERMINAL       \n";
    
    std::cout << "-----------------------------------------\n\n";

    while (true) {
        std::string code;
        std::cout << "terminal > ";  
        std::getline(std::cin, code);

        if (code == "exit" || code == "quit") {
            std::cout << "program exited\n";
            break;
        }
        if (code.empty()) continue;

        runPipeline(code, showAST, showBytecode, stepLimit);
    }

    return 0;
}
