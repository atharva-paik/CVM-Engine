<div align="center">

# CVM++ Engine
### A Custom Stack-Based Virtual Machine and Compiler Pipeline built from scratch in C++.

</div>

---

## 1. What is Our Project?

**CVM++ Engine** is a custom, Turing-complete scripting language and execution environment designed and implemented entirely from scratch in modern C++. The project demonstrates how a high-level source program can move through a complete compiler pipeline and ultimately execute on a custom runtime. Instead of relying on parser generators or external language tooling, CVM++ uses a fully manual 4-stage architecture: a hand-rolled **Lexer** converts raw source code into tokens, a **Recursive Descent Parser** transforms those tokens into an **Abstract Syntax Tree (AST)**, a **Compiler** flattens the AST into a custom **Bytecode** instruction stream, and a register-free **Stack-Based Virtual Machine** executes those instructions deterministically.

---

## 2. Problem Statement & Deliverables

### Problem Statement

The goal of this Even Semester Project was to build a complete compiler and runtime pipeline in order to deeply understand **low-level execution**, **language design**, **control flow**, **memory handling**, and **program interpretation/compilation**. A core requirement of the project was to avoid external parser-generators such as **Lex/Yacc** and to avoid dependence on third-party libraries, ensuring that every major subsystem was implemented directly in C++.

### Deliverables Achieved

1. **Full C++ source code** for the language pipeline and runtime.
2. **A custom instruction set (bytecode)** for compiled program execution.
3. **Debug flags** to visualize both the **AST** and generated **Bytecode**.
4. **Execution of Turing-complete logic**, including programs such as **truth machines** and **FizzBuzz**.
5. **Documentation and a demo video** for presentation and evaluation.

---

## 3. What We Implemented (Features & Architecture)

### Language Features

- **Variables** using `let` bindings and reassignment.
- **Mathematical operations** with correct **PEMDAS**-style precedence handling.
- **Boolean logic and comparisons** such as equality and relational checks.
- **Conditional branching** through `if/else` statements.
- **Loops** using `while` statements with compiler-side jump patching.
- **Built-in I/O** through `print` and `input`.

### Core Architecture

The engine follows a clean compiler-runtime pipeline:

`Lexer -> Parser -> Compiler -> VM`

- **Lexer**: Converts source text into tokens.
- **Parser**: Builds an AST using recursive descent parsing.
- **Compiler**: Emits custom bytecode from the AST.
- **VM**: Executes bytecode on a register-free stack machine.

---

## 4. Prerequisites

Before running the project locally, make sure the following tools are installed:

- **Git**: Required for cloning the repository.  
  Download: [https://git-scm.com/downloads](https://git-scm.com/downloads)

- **C++ Compiler (GCC/MinGW)**: A compiler with **C++17** support or higher.  
  Download options: [TDM-GCC](https://jmeubank.github.io/tdm-gcc/) or [MSYS2](https://www.msys2.org/)

---

## 5. Complete Setup Steps (From Clone to Local)

Use the following commands to clone and build the project locally:

```bash
git clone https://github.com/atharva-paik/CVM-Engine.git
cd CVM-Engine
g++ src/main.cpp -o cvm
```

The compiled executable, `cvm`, will be generated in the **root directory** of the project.

---

## 6. Complete Set of Commands (How to Use)

### 6.1 Interactive Terminal (REPL Mode)

Running the executable without arguments launches the built-in interactive terminal, allowing you to type and execute CVM++ statements live.

```bash
.\cvm
```

Example session:

```cpp
let x = 10;
print x * 2;
```

### 6.2 Running a Script

To execute a `.cvm` source file directly, pass the script path as an argument:

```bash
.\cvm tests\test_fizzbuzz.cvm
```

### 6.3 Debugging (The AST and Bytecode)

CVM++ provides custom debug flags to inspect the internal compilation process. The following command prints the **Abstract Syntax Tree**, then the generated **Bytecode**, and finally the program's execution output:

```bash
.\cvm --ast --bytecode tests\test_arithmetic.cvm
```

These flags are especially useful for understanding how source code is transformed at each stage of the pipeline.

---

## 7. Repository Structure

Below is the high-level layout of the repository:

```text
CVM-Engine/
|-- README.md
|-- instructions.txt
|-- src/
|   |-- main.cpp
|   |-- lexer.h
|   |-- Parser.h
|   |-- ast.h
|   |-- compiler.h
|   |-- virtualmachine.h
|-- tests/
|   |-- test_arithmetic.cvm
|   |-- test_booleans.cvm
|   |-- test_fizzbuzz.cvm
|   `-- test_if_else.cvm
```

- **`src/`** contains the full compiler pipeline and virtual machine implementation.
- **`tests/`** contains sample CVM++ programs used for validation and demonstration.

---

This repository showcases the design and implementation of a compact but complete language system, from tokenization to execution. CVM++ Engine is intended as both a working virtual machine and a learning-oriented compiler project that demonstrates how programming languages can be built from first principles in C++.
