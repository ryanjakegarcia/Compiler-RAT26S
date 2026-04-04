# RAT26S Parser Project

Simple C++ lexer/parser project for RAT26S grammar with a batch test driver.

## Build and Run (Linux)
Build: g++ -std=c++17 -O2 -Isrc src/main.cpp src/lexer.cpp src/parser.cpp -o bin/parser_batch
Run: ./bin/parser_batch

## Build (Windows .exe)
Build: x86_64-w64-mingw32-g++ -std=c++17 -O2 -Isrc src/main.cpp src/lexer.cpp src/parser.cpp -static -static-libgcc -static-libstdc++ -o bin/parser_batch.exe
Run on Windows: parser_batch.exe

## Test Layout
Put expected-valid .rat files in pass/ and expected-invalid .rat files in fail/.
The driver reports pass/fail based on expected outcome and writes .parse.txt files next to inputs.
