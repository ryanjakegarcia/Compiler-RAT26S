#pragma once
#include "lexer.h"
#include <fstream>

class Parser{
private:
    Lexer& lexer;
    Token currentToken;
    std::ofstream outputFile;
    bool traceOutput = true;

    //helpers
    void advance();
    void expect(const std::string& expectedLexeme);
    void printProduction(const std::string& ruleName);
    void syntaxError(const std::string& message);

    //Grammar rules
    void rat26s();
    void optFunctionDefinitions();
    void functionDefinitions();
    void function();
    void optParameterList();
    void parameterList();
    void parameter();
    void qualifier();
    void body();
    void optDeclarationList();
    void declarationList();
    void declaration();
    void ids();
    void statementList();
    void statement();
    void compound();
    void assign();
    void ifStatement();
    void ifTail();
    void returnStatement();
    void returnTail();
    void print();
    void scan();
    void whileStatement();
    void condition();
    void relop();
    void expression();
    void expressionTail();
    void term();
    void termTail();
    void factor();
    void primary();
    void primaryTail();

public:
    Parser(Lexer& lex, const std::string& outputFilename);
    ~Parser();
    void parse();
};

