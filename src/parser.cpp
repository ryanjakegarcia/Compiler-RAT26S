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
    throw std::runtime_error(
        "Syntax Error (line " + std::to_string(currentToken.line) + "): " +
        message +
        " Token: " + std::string(currentToken.typeToString()) +
        " Lexeme: " + currentToken.lexeme
    );
}

bool Parser::isQualifier(const std::string& lexeme) const {
    return lexeme == "integer" || lexeme == "boolean" || lexeme == "real";
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

void Parser::parameterList(){
    parameter();
    if(currentToken.lexeme == ","){
        printProduction("<Parameter List> -> <Parameter> , <Parameter List>");
        expect(",");
        parameterList();
    }
    else
        printProduction("<Parameter List> -> <Parameter>");
}

void Parser::parameter(){
    printProduction("<Parameter> -> <IDs> <Qualifier>");
    ids();
    qualifier();
}

void Parser::qualifier(){
    printProduction("<Qualifier> -> integer | boolean | real");
    if(isQualifier(currentToken.lexeme)){
        advance();
    }
    else{
        syntaxError("Expected qualifier");
    }
}

void Parser::body(){
    printProduction("<Body> -> { <Statement List> }");
    expect("{");
    statementList();
    expect("}");
}

void Parser::optDeclarationList(){
    if(isQualifier(currentToken.lexeme)){
        printProduction("<Opt Declaration List> -> <Declaration List>");
        declarationList();
    }
    else{
        printProduction("<Opt Declaration List> -> <Empty>");
    }
}

void Parser::declarationList(){
    declaration();
    expect(";");
    if(isQualifier(currentToken.lexeme)){
        printProduction("<Declaration List> -> <Declaration> ; <Declaration List>");
        declarationList();
    }
    else{
        printProduction("<Declaration List> -> <Declaration> ;");
    }
}

void Parser::declaration(){
    printProduction("<Declaration> -> <Qualifier> <IDs>");
    qualifier();
    ids();
}

void Parser::ids(){
    if(currentToken.type == TokenType::IDENTIFIER){
        advance();
        if(currentToken.lexeme == ","){
            printProduction("<IDs> -> <Identifier> , <IDs>");
            expect(",");
            ids();
        }
        else{
            printProduction("<IDs> -> <Identifier>");
        }
    }
    else{
        syntaxError("Expected Identifier");
    }
}

void Parser::statementList(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::statement(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::compound(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::assign(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::ifStatement(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::ifTail(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::returnStatement(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::returnTail(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::print(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::scan(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::whileStatement(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::condition(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::relop(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::expression(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::expressionTail(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::term(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::termTail(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::factor(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::primary(){
    throw std::runtime_error("Not yet implemented");
}

void Parser::primaryTail(){
    throw std::runtime_error("Not yet implemented");
}

