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


void runPipeline(std::string code) {
    try {
        Lexer lexer(code);
        Parser parser(lexer.tokenize());
        Compiler compiler;
        VM vm;
        
        std::vector<int> bytecode = compiler.compile(parser.parse());
        vm.load(bytecode);
        vm.run(); 
        
    } catch (const std::exception& e) {
        std::cout << e.what() << "\n";
    } catch (...) {
        std::cout << "Syntax Error: Unrecognized command or structure.\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string filename = argv[1];
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "Error: Could not open file '" << filename << "'\n";
            return 1;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf(); 
        
        std::cout << "Running " << filename << "...\n";
        std::cout << "-----------------------------------------\n";
        runPipeline(buffer.str());
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

        runPipeline(code);
    }

    return 0;
}