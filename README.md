# CVM++ : Custom Compiler & Stack-Based Virtual Machine

## Overview
CVM++ is a custom, Turing-complete scripting language and stack-based virtual machine built entirely from scratch in standard C++. 

Designed to demonstrate low-level memory architecture and compiler theory, the engine implements a complete execution pipeline without reliance on external parser-generators (such as ANTLR) or third-party libraries. 

## System Architecture
The execution pipeline is divided into four primary stages:

1. **Lexical Analysis:** Tokenizes raw source code strings, identifying keywords, mathematical operators, and identifiers while managing whitespace.
2. **Parsing (AST Generation):** Utilizes a custom Recursive Descent Parser to enforce mathematical precedence (PEMDAS) and construct the Abstract Syntax Tree (AST).
3. **Bytecode Compilation:** Traverses the AST to generate a linear stream of custom bytecode instructions. Implements forward and backward jump-patching to handle `while` loops and conditional branching.
4. **Virtual Machine Execution:** A register-free execution engine operating on a custom, strictly typed integer memory stack. 

*Technical Note on Memory:* As the VM utilizes a strictly typed integer stack, boolean truth values are evaluated and pushed directly to memory as `1` (true) and `0` (false), mirroring native C-level boolean evaluation.

## Build Instructions
CVM++ requires no external dependencies. To compile the engine from source, use standard `g++`.

Clone the repository and run the following command in the root directory:
```bash
g++ src/main.cpp -o cvm