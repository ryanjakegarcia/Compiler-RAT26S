#include "lexer.h" // uses std namespace

void printLexerOutput(Lexer &lexer, Token t){
  do {
    t = lexer.getNextToken();
    if (t.type != TokenType::END_OF_FILE) {
      std::cout << std::left << std::setw(15) << t.typeToString() << t.lexeme << std::endl;
    }
  } while (t.type != TokenType::END_OF_FILE);
};

int main() {
  Lexer lexer1("firstTest.rat");
  Token t;

  std::cout << "Printing output for firstTest.rat" << std::endl;
  std::cout << std::left << std::setw(15) << "token" << "lexeme" << std::endl;
  std::cout << "-------------------------------" << std::endl;

  printLexerOutput(lexer1, t);


  Lexer lexer2("secondTest.rat");
  
  std::cout << std::endl << std::endl;
  
  std::cout << "Printing output for secondTest.rat" << std::endl;
  std::cout << std::left << std::setw(15) << "token" << "lexeme" << std::endl;
  std::cout << "-------------------------------" << std::endl;
  
  printLexerOutput(lexer2, t);


  Lexer lexer3("thirdTest.rat");
  
  std::cout << std::endl << std::endl;
  
  std::cout << "Printing output for thirdTest.rat" << std::endl;
  std::cout << std::left << std::setw(15) << "token" << "lexeme" << std::endl;
  std::cout << "-------------------------------" << std::endl;
  
  printLexerOutput(lexer3, t);

  return 0;
}
