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
    outputFile << " Token: " << currentToken.typeToString() << " Lexeme: " << currentToken.lexeme << '\n';
}

void Parser::printProduction(const std::string& ruleName){
    if(traceOutput)
        outputFile << ruleName << '\n';
}

void Parser::syntaxError(const std::string& message){
    throw std::runtime_error(message + " Token: " + std::string(currentToken.typeToString()) + " Lexeme: " + currentToken.lexeme);
}

void Parser::expect(const std::string& expectedLexeme){
    if(currentToken.lexeme == expectedLexeme)
        advance();
    else
        syntaxError("Expected: " + expectedLexeme + " Found: " + currentToken.lexeme);
}

void Parser::parse(){
    advance();
    rat26s();
    if(currentToken.type != TokenType::END_OF_FILE)
        syntaxError("Expected EOF ");
    else
        outputFile << "Successfuly parsed file!" << '\n';
}

void Parser::rat26s(){
    printProduction("<Rat26S> -> @ <Opt Function Definitions> @ <Opt Declaration List> @ <Statement List> @");
    expect("@");
    optFunctionDefinitions();
    expect("@");
    optDeclarationList();
    expect("@");
    statementList();
    expect("@");
}

void Parser::optFunctionDefinitions(){
    if(currentToken.lexeme == "function"){
        printProduction("<Opt Function Definitions> -> <Function Definitions>");
        functionDefinitions();   
    }
    else{
        printProduction("<Opt Function Definitions> -> <Empty>");
    }
}

void Parser::functionDefinitions(){
    function();
    if(currentToken.lexeme == "function"){
        printProduction("<Function Definitions> -> <Function> <Function Definitions>");
        functionDefinitions();
    }
    else
        printProduction("<Function Definitions> -> <Function>");
}

void Parser::function(){
    printProduction("<Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
    expect("function");
    if(currentToken.type != TokenType::IDENTIFIER){
        syntaxError("[function] expected identifier, found token=" + std::string(currentToken.typeToString()));
    }

    advance();
    expect("(");
    optParameterList();
    expect(")");
    optDeclarationList();
    body();
}

void Parser::optParameterList(){
    if(currentToken.lexeme == ")"){
        printProduction("<Opt Parameter List> -> <Empty>");
    }
    else{
        printProduction("<Opt Parameter List> -> <Parameter List>");
        parameterList();
    }
}

