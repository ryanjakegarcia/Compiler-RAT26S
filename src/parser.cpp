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
    return lexeme == "integer" ||
           lexeme == "boolean" ||
           lexeme == "real";
}

bool Parser::isStatement() const {
    return currentToken.lexeme == "{" ||
           currentToken.lexeme == "if" ||
           currentToken.lexeme == "return" ||
           currentToken.lexeme == "write" ||
           currentToken.lexeme == "read" ||
           currentToken.lexeme == "while" ||
           currentToken.type == TokenType::IDENTIFIER;
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
    if(!isStatement())
        syntaxError("Expected start of statement list");

    statement();

    if(isStatement()){
        printProduction("<Statement List> -> <Statement> <Statement List>");
        statementList();
    }
    else{
        printProduction("<Statement List> -> <Statement>");
    }
}

void Parser::statement(){
    if(currentToken.lexeme == "{"){
        printProduction("<Statement> -> <Compound>");
        compound();
    }
    else if(currentToken.lexeme == "if"){
        printProduction("<Statement> -> <If>");
        ifStatement();
    }
    else if(currentToken.lexeme == "return"){
        printProduction("<Statement> -> <Return>");
        returnStatement();
    }
    else if(currentToken.lexeme == "write"){
        printProduction("<Statement> -> <Print>");
        print();
    }
    else if(currentToken.lexeme == "read"){
        printProduction("<Statement> -> <Scan>");
        scan();
    }
    else if(currentToken.lexeme == "while"){
        printProduction("<Statement> -> <While>");
        whileStatement();
    }
    else if(currentToken.type == TokenType::IDENTIFIER){
        printProduction("<Statement> -> <Assign>");
        assign();
    }
    else{
        syntaxError("Expected statement");
    }
}

void Parser::compound(){
    printProduction("<Compound> -> { <Statement List> }");
    expect("{");
    statementList();
    expect("}");
}

void Parser::assign(){
    if(currentToken.type == TokenType::IDENTIFIER){
        printProduction("<Assign> -> <Identifier> = <Expression> ;");
        advance();
        expect("=");
        expression();
        expect(";");
    }
    else{
        syntaxError("Expected Identifier");
    }
}

void Parser::ifStatement(){
    printProduction("<If> -> if ( <Condition> ) <Statement> <If Tail>");
    expect("if");
    expect("(");
    condition();
    expect(")");
    statement();
    ifTail();
}

void Parser::ifTail(){
    if(currentToken.lexeme == "fi"){
        printProduction("<If Tail> -> fi");
        expect("fi");
    }
    else if(currentToken.lexeme == "otherwise"){
        printProduction("<If Tail> -> otherwise <Statement> fi");
        expect("otherwise");
        statement();
        expect("fi");
    }
    else{
        syntaxError("Expected 'fi' or 'otherwise'");
    }
}

void Parser::returnStatement(){
    printProduction("<Return> -> return <Return Tail>");
    expect("return");
    returnTail();
}

void Parser::returnTail(){
    if(currentToken.lexeme == ";"){
        printProduction("<Return Tail> -> ;");
        expect(";");
    }
    else{
        printProduction("<Return Tail> -> <Expression> ;");
        expression();
        expect(";");
    }
}

void Parser::print(){
    printProduction("<Print> -> write ( <Expression> ) ;");
    expect("write");
    expect("(");
    expression();
    expect(")");
    expect(";");
}

void Parser::scan(){
    printProduction("<Scan> -> read ( <IDs> ) ;");
    expect("read");
    expect("(");
    ids();
    expect(")");
    expect(";");
}

void Parser::whileStatement(){
    printProduction("<While> -> while ( <Condition> ) <Statement>");
    expect("while");
    expect("(");
    condition();
    expect(")");
    statement();
}

void Parser::condition(){
    printProduction("<Condition> -> <Expression> <Relop> <Expression>");
    expression();
    relop();
    expression();
}

void Parser::relop(){
    printProduction("<Relop> -> == | != | > | < | <= | >=");
    if(currentToken.lexeme == "==") {
        expect("==");
    }
    else if(currentToken.lexeme == "!="){
        expect("!=");
    }
    else if(currentToken.lexeme == ">"){
        expect(">");
    }
    else if(currentToken.lexeme == "<"){
        expect("<");
    }
    else if(currentToken.lexeme == "<="){
        expect("<=");
    }
    else if(currentToken.lexeme == ">="){
        expect(">=");
    }
    else{
        syntaxError("Expected relational operator");
    }
}

void Parser::expression(){
    printProduction("<Expression> -> <Term> <Expression Tail>");
    term();
    expressionTail();
}

void Parser::expressionTail(){
    if(currentToken.lexeme == "+"){
        printProduction("<Expression Tail> -> + <Term> <Expression Tail>");
        expect("+");
        term();
        expressionTail();
    }
    else if(currentToken.lexeme == "-"){
        printProduction("<Expression Tail> -> - <Term> <Expression Tail>");
        expect("-");
        term();
        expressionTail();
    }
    else{
        printProduction("<Expression Tail> -> <Empty>");
    }
}

void Parser::term(){
    printProduction("<Term> -> <Factor> <Term Tail>");
    factor();
    termTail();
}

void Parser::termTail(){
    if(currentToken.lexeme == "*"){
        printProduction("<Term Tail> -> * <Factor> <Term Tail>");
        expect("*");
        factor();
        termTail();
    }
    else if(currentToken.lexeme == "/"){
        printProduction("<Term Tail> -> / <Factor> <Term Tail>");
        expect("/");
        factor();
        termTail();
    }
    else{
        printProduction("<Term Tail> -> Empty");
    }
}

void Parser::factor(){
    if(currentToken.lexeme == "-"){
        printProduction("<Factor> -> - <Primary>");
        expect("-");
        primary();
    }
    else{
        printProduction("<Factor> -> <Primary>");
        primary();
    }
}

void Parser::primary(){
    if(currentToken.type == TokenType::IDENTIFIER){
        printProduction("<Primary> -> <Identifier> <Primary Tail>");
        advance();
        primaryTail();
    }
    else if(currentToken.type == TokenType::INTEGER){
        printProduction("<Primary> -> <Integer>");
        advance();
    }
    else if(currentToken.type == TokenType::REAL){
        printProduction("<Primary> -> <Real>");
        advance();
    }
    else if(currentToken.lexeme == "true"){
        printProduction("<Primary> -> true");
        expect("true");
    }
    else if(currentToken.lexeme == "false"){
        printProduction("<Primary> -> false");
        expect("false");
    }
    else if(currentToken.lexeme == "("){
        printProduction("<Primary> -> ( <Expression> )");
        expect("(");
        expression();
        expect(")");
    }
    else{
        syntaxError("Expected Primary Start");
    }
}

void Parser::primaryTail(){
    if(currentToken.lexeme == "("){
        printProduction("<Primary Tail> -> ( <IDs> )");
        expect("(");
        ids();
        expect(")");
    }
    else
        printProduction("<Primary Tail> -> <Empty>");
}

