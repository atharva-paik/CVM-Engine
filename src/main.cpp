//Connected compiler pipeline in main entry point
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer.h" 
#include "ast.h"
#include "Parser.h" 
#include "compiler.h" 
#include "virtualmachine.h" 


void runPipeline(const std::string& code, bool printBytecode = false, bool printAst = false) {
    try {
        Lexer lexer(code);
        Parser parser(lexer.tokenize());
        Compiler compiler;
        VM vm;
        std::vector<std::shared_ptr<ASTNode>> program = parser.parse();
        
        if (printAst) {
            for (const auto& node : program) {
                if (node) {
                    node->print(0);
                }
            }
        }

        std::vector<int> bytecode = compiler.compile(program);
        if (printBytecode) {
            compiler.printBytecode();
        }
        vm.load(bytecode);
        vm.run(); 
        
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    } catch (...) {
        std::cout << "Syntax Error: Unrecognized command or structure.\n";
    }
}

int main(int argc, char* argv[]) {
    bool printBytecode = false;
    bool printAst = false;
    std::string filename;

    for (int i = 1; i < argc; ++i) {
        std::string argument = argv[i];

        if (argument == "--bytecode") {
            printBytecode = true;
        } else if (argument == "--ast") {
            printAst = true;
        } else if (filename.empty()) {
            filename = argument;
        } else {
            std::cout << "Error: Unexpected argument '" << argument << "'\n";
            return 1;
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
        runPipeline(buffer.str(), printBytecode, printAst);
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

        runPipeline(code, printBytecode, printAst);
    }

    return 0;
}
