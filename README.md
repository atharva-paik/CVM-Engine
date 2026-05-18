# CVM++

**A custom C++ scripting language that compiles source code into bytecode and executes it on a stack-based virtual machine.**

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=c%2B%2B)
![Build](https://img.shields.io/badge/build-g%2B%2B-success?style=flat-square)
![Runtime](https://img.shields.io/badge/runtime-stack%20VM-blue?style=flat-square)
![Status](https://img.shields.io/badge/status-working%20prototype-brightgreen?style=flat-square)

## Overview

**CVM++** is a lightweight compiler and virtual machine project built from scratch in C++. It demonstrates the complete execution pipeline of a small programming language: lexical analysis, parsing, AST generation, bytecode compilation, and stack-based virtual machine execution.

The project was developed for the **CVM++: Stack-Based Virtual Machine & Custom Compiler** requirement from `EvenSemProjects.pdf`. It is intentionally implemented without parser generators or external compiler frameworks, making each stage of the language pipeline visible and easy to inspect.

## Core Pipeline

```text
Source Code (.cvm)
        |
        v
Lexer -> Tokens
        |
        v
Parser -> Abstract Syntax Tree
        |
        v
Compiler -> Bytecode
        |
        v
Stack-Based Virtual Machine
        |
        v
Program Output
```

## Features

- Hand-written lexer for keywords, identifiers, operators, literals, braces, parentheses, semicolons, and comments.
- Recursive descent parser with arithmetic and comparison precedence.
- Abstract Syntax Tree representation for expressions, statements, variables, blocks, loops, input, and output.
- Custom bytecode compiler using integer opcodes.
- Stack-based virtual machine with variable memory slots.
- File runner for `.cvm` programs.
- Interactive terminal for quick one-line execution.
- Optional AST and bytecode debug output.
- Runtime protection for division by zero.
- Runtime step limit to stop accidental infinite loops.
- Support for negative integer expressions such as `-5`, `-x`, and `x * -3`.

## Language Support

CVM++ currently supports:

| Feature | Example |
|---|---|
| Integer literals | `10`, `0`, `-5` |
| Boolean literals | `true`, `false` |
| Variable declaration | `let x = 10;` |
| Assignment | `x = x + 1;` |
| Arithmetic | `+`, `-`, `*`, `/` |
| Comparisons | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| If/else | `if (x == 1) { print 1; } else { print 0; }` |
| While loops | `while (x > 0) { print x; x = x - 1; }` |
| Input | `input value;` |
| Output | `print value;` |
| Comments | `// this is a comment` |

Booleans are represented internally as integers: `true` is `1`, and `false` is `0`.

## Build

From the project root:

```powershell
g++ -std=c++17 src/main.cpp -o cvm_new.exe
```

This creates a Windows executable named `cvm_new.exe`.

## Usage

Run a `.cvm` script:

```powershell
.\cvm.exe tests\test_arithmetic.cvm
```

Run with full debug output:

```powershell
.\cvm.exe tests\test_arithmetic.cvm --debug
```

Debug mode prints:

1. The generated AST.
2. The compiled bytecode.
3. The final VM output.

Run only AST output:

```powershell
.\cvm.exe tests\test_arithmetic.cvm --ast
```

Run only bytecode output:

```powershell
.\cvm.exe tests\test_arithmetic.cvm --bytecode
```

Run with a custom VM step limit:

```powershell
.\cvm.exe tests\test_truth_machine.cvm --max-steps 40
```

## Example Program

```c
let a = 10;
let b = 20;

let sum = a + b;
let neg = -a;
let complex = (a + 5) * 2;

print sum;
print neg;
print complex;
```

Expected output:

```text
Your output: 30
Your output: -10
Your output: 30
```

## Debug Example

Command:

```powershell
.\cvm.exe tests\test_if_else.cvm --debug
```

Example output sections:

```text
===== AST =====
...
===== BYTECODE =====
...
output -> Your output: 999
Your output: 888
```

## Test Programs

The `tests/` directory includes sample programs for each major language feature:

```text
tests/
|-- test_arithmetic.cvm       # Arithmetic, variables, unary minus
|-- test_booleans.cvm         # Boolean and comparison operations
|-- test_if_else.cvm          # Conditional execution
|-- test_fizzbuzz.cvm         # Loops and nested conditionals
|-- test_input.cvm            # Runtime integer input
|-- test_truth_machine.cvm    # Input and loop safety behavior
`-- test_div_zero.cvm         # Runtime division-by-zero handling
```

## Project Structure

```text
cvm++project-main/
|-- README.md
|-- instructions.txt
|-- src/
|   |-- main.cpp              # CLI runner, debug flags, REPL entry
|   |-- lexer.h               # Tokenizer
|   |-- Parser.h              # Recursive descent parser
|   |-- ast.h                 # AST nodes and AST printer
|   |-- compiler.h            # AST-to-bytecode compiler
|   `-- virtualmachine.h      # Stack-based VM and bytecode printer
`-- tests/
    |-- test_arithmetic.cvm
    |-- test_booleans.cvm
    |-- test_if_else.cvm
    |-- test_fizzbuzz.cvm
    |-- test_input.cvm
    |-- test_truth_machine.cvm
    `-- test_div_zero.cvm
```

## Technical Highlights

- **Parsing strategy:** recursive descent parser.
- **Intermediate representation:** AST followed by custom bytecode.
- **Execution model:** stack machine with integer memory slots.
- **Control flow:** jump-based bytecode for `if/else` and `while`.
- **Error handling:** division-by-zero runtime error and step-limit protection.

## Current Limitations

- Runtime values are integer-only.
- Strings, arrays, functions, and lexical scopes are not implemented.
- The interactive terminal evaluates each entered line as a separate program, so variables do not persist across prompts.
- The VM uses a fixed-size integer memory array.

