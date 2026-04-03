#include "parser.h"
#include <stdexcept>
#include <iostream>


Parser::Parser(Lexer& lex, const std::string& outputFilename) 
    : lexer(lex), outputFile(outputFilename){
    if(!outputFile.is_open()){
        throw std::runtime_error("Failed to open parser output file: " + outputFilename);
    }
}

Parser::~Parser(){
    if(outputFile.is_open())
        outputFile.close();
}

void Parser::advance(){
    currentToken = lexer.getNextToken();
    outputFile << "Token: " << currentToken.typeToString() << " Lexeme: " << currentToken.lexeme << '\n';
}

void Parser::printProduction(const std::string& ruleName){
    if(traceOutput)
        outputFile << ruleName << '\n';
}

void Parser::syntaxError(const std::string& message){
    throw std::runtime_error(message + "Token: " + std::string(currentToken.typeToString()) + " Lexeme: " + currentToken.lexeme);
}

void Parser::expect(const std::string& expectedLexeme){
    if(currentToken.lexeme == expectedLexeme)
        advance();
    else
        syntaxError("Expected: " + expectedLexeme + " Found: " + currentToken.lexeme);
}
