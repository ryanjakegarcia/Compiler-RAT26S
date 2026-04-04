# RAT26S Parser Project

**CPSC 323 Compiler Project Assignment 2**

A C++ implementation of a lexer and recursive-descent parser for the RAT26S language specification, with batch test harness.

## Overview

RAT26S is a simple procedural language supporting:
- Variable declarations (integer, real, boolean)
- Functions with parameters and return statements
- Control flow (if/otherwise, while loops)
- Expressions with arithmetic and relational operators
- I/O statements (read, write)

The parser validates syntax and generates a derivation trace during parsing.

## Architecture

**Lexer** (`src/lexer.cpp`, `src/lexer.h`)
- Tokenizes input into meaningful symbols
- Recognizes keywords, identifiers, numbers, separators, operators

**Parser** (`src/parser.cpp`, `src/parser.h`)
- Recursive-descent parser implementing the RAT26S grammar
- ~33 parsing functions, one per major grammar rule
- On syntax error, throws exception with line number, expected token, and actual found token
- On success, generates a `.parse.txt` trace file

**Driver** (`src/main.cpp`)
- Batch harness that runs parser over test suites
- Scans `pass/` for expected-success files and `fail/` for expected-error files
- Reports pass/fail per file against expected outcomes
- Colorizes error diagnostics (bold red) in terminal output

## Build and Run (Linux)

Build:
```bash
g++ -std=c++17 -O2 -Isrc src/main.cpp src/lexer.cpp src/parser.cpp -o bin/parser_batch
```

Run:
```bash
./bin/parser_batch
```

## Build and Run (Windows)

Build executable:
```bash
g++ -std=c++17 -O2 -Isrc src/main.cpp src/lexer.cpp src/parser.cpp -o bin/parser_batch.exe
```

Run on Windows:
```bash
.\bin\parser_batch.exe
```

## Test Layout

- **pass/** - Valid RAT26S programs; parser should succeed on all
- **fail/** - Invalid RAT26S programs; parser should reject each with a specific error

The driver writes a `.parse.txt` derivation file next to each input.
Exit code is the number of failed tests (unexpected outcomes). `0` means all tests behaved as expected.

## RAT26S Production Rules

```text
R1.  <Rat26S> ::= @ <Opt Function Definitions> @ <Opt Declaration List> @ <Statement List> @
R2.  <Opt Function Definitions> ::= <Function Definitions> | <Empty>
R3.  <Function Definitions> ::= <Function> | <Function> <Function Definitions>
R4.  <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
R5.  <Opt Parameter List> ::= <Parameter List> | <Empty>
R6.  <Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>
R7.  <Parameter> ::= <IDs> <Qualifier>
R8.  <Qualifier> ::= integer | boolean | real
R9.  <Body> ::= { <Statement List> }
R10. <Opt Declaration List> ::= <Declaration List> | <Empty>
R11. <Declaration List> ::= <Declaration> ; | <Declaration> ; <Declaration List>
R12. <Declaration> ::= <Qualifier> <IDs>
R13. <IDs> ::= <Identifier> | <Identifier> , <IDs>
R14. <Statement List> ::= <Statement> | <Statement> <Statement List>
R15. <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
R16. <Compound> ::= { <Statement List> }
R17. <Assign> ::= <Identifier> = <Expression> ;
R18. <If> ::= if ( <Condition> ) <Statement> <If Tail>
R19. <If Tail> ::= fi | otherwise <Statement> fi
R20. <Return> ::= return <Return Tail>
R21. <Return Tail> ::= ; | <Expression> ;
R22. <Print> ::= write ( <Expression> ) ;
R23. <Scan> ::= read ( <IDs> ) ;
R24. <While> ::= while ( <Condition> ) <Statement>
R25. <Condition> ::= <Expression> <Relop> <Expression>
R26. <Relop> ::= == | != | > | < | <= | =>
R27. <Expression> ::= <Term> <Expression Tail>
R28. <Expression Tail> ::= + <Term> <Expression Tail> | - <Term> <Expression Tail> | <Empty>
R29. <Term> ::= <Factor> <Term Tail>
R30. <Term Tail> ::= * <Factor> <Term Tail> | / <Factor> <Term Tail> | <Empty>
R31. <Factor> ::= - <Primary> | <Primary>
R32. <Primary> ::= <Identifier> <Primary Tail> | <Integer> | ( <Expression> ) | <Real> | true | false
R33. <Primary Tail> ::= <Empty> | ( <IDs> )
R34. <Empty> ::= Epsilon
```
