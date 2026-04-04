#include "parser.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    try {
        Lexer lex(argv[1]);
        Parser p(lex, "testcases/out.parse.txt");
        p.parse();
        std::cout << "parse ok\n";
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }
}
