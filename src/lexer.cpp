#include "lexer.h"

// Private Functions
void Lexer::nextChar() {
    if (!sourceFile.get(currentChar)) {
        currentChar = EOF;
    }
};

char Lexer::peek() {
    return sourceFile.peek();
};

void Lexer::skipWhitespaceAndComments() {
    while (isspace(currentChar) || currentChar == '/') {
        if (isspace(currentChar)) {
            nextChar();
        } else if (currentChar == '/' && peek() == '*') {
            // Skip Comment /* ... */
            nextChar(); // consume '/'
            nextChar(); // consume '*'
            while (currentChar != EOF) {
                if (currentChar == '*' && peek() == '/') {
                    nextChar(); // consume '*'
                    nextChar(); // consume '/'
                    break;
                }
                nextChar();
            }
        }
        else {
            break; 
        }
    }
};

bool Lexer::isSeparator(char c) {
    std::string separators = "(){}[],;@";
    return separators.find(c) != std::string::npos;
};

bool Lexer::isOperator(char c) {
    std::string operators = "=+-*/<>";
    return operators.find(c) != std::string::npos;
};


// Public Functions
Lexer::Lexer(std::string filename) {
    sourceFile.open(filename);
    if (!sourceFile.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
    }
    nextChar();
};

Token Lexer::getNextToken() {
  skipWhitespaceAndComments();

  if (currentChar == EOF) {
    return {TokenType::END_OF_FILE, "EOF"};
  }

  // FSM for identifiers and keywords
  if (isalpha(currentChar)) {
    std::string buffer;
    while (isalnum(currentChar) || currentChar == '_') {
      buffer += tolower(currentChar);
      nextChar();
    }
    if (keywords.count(buffer)) {
      return {TokenType::KEYWORD, buffer};
    }
    return {TokenType::IDENTIFIER, buffer};
  }

  // FSM for integers and reals
  if (isdigit(currentChar)) {
    std::string buffer;
    bool isReal = false;

    while (isdigit(currentChar)) {
      buffer += currentChar;
      nextChar();
    }

    if (currentChar == '.') {
      isReal = true;
      buffer += currentChar;
      nextChar();
      while (isdigit(currentChar)) {
        buffer += currentChar;
        nextChar();
      }
    }

    return {isReal ? TokenType::REAL : TokenType::INTEGER, buffer};
  }

  // handle separators
  if (isSeparator(currentChar)) {
    std::string s(1, currentChar);
    nextChar();
  
    return {TokenType::SEPARATOR, s};
  }

  // operator logic
  if (isOperator(currentChar) || currentChar == '!') {
    std::string op(1, currentChar);
    char first = currentChar;
    nextChar();
  
    // Check if the next character forms a valid operator with length=2
    if (first == '=' && currentChar == '=') {
      op += currentChar;
      nextChar();
    } else if (first == '!' && currentChar == '=') {
      op += currentChar;
      nextChar();
    } else if (first == '<' && currentChar == '=') {
      op += currentChar;
      nextChar();
    } else if (first == '>' && currentChar == '=') {
      op += currentChar;
      nextChar();
    } 
    // account for case where just '!' is encountered
    else if (first == '!' && currentChar != '=') {
      return {TokenType::ERROR, "!"};
    }
  
    return {TokenType::OPERATOR, op};
  }

  // error handling
  std::string err(1, currentChar);
  nextChar();
  
  return {TokenType::ERROR, err};
};
