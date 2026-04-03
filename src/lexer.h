#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <cctype>
#include <unordered_set>
#include <iomanip>
#include <array>
#include <cstdint>

//using namespace std;
using u8 = std::uint8_t;

enum class TokenType : u8 {
    IDENTIFIER,
    KEYWORD,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
    END_OF_FILE,
    ERROR,
    Count
};

constexpr std::array<std::string_view, static_cast<std::size_t>(TokenType::Count)>
TokenTypeNames = {
    "IDENTIFIER",
    "KEYWORD",
    "INTEGER",
    "REAL",
    "OPERATOR",
    "SEPARATOR",
    "EOF",
    "ERROR"
};

constexpr std::string_view tokenTypeToString(TokenType type) noexcept {
    const auto idx = static_cast<std::size_t>(type);
    return (idx < TokenTypeNames.size()) ? TokenTypeNames[idx] : "UNKNOWN";
}

struct Token {
    TokenType type = TokenType::ERROR;
    std::string lexeme;

    std::string_view typeToString() const noexcept {
        return tokenTypeToString(type);
    }
};

class Lexer {
  private:
    std::ifstream sourceFile;
    char currentChar;
    std::unordered_set<std::string> keywords = {
        "integer", "if", "otherwise", "fi", "while", 
        "return", "read", "write", "function", "boolean", "real"
    };
    
    void nextChar();
    char peek();
    void skipWhitespaceAndComments();
    bool isSeparator(char c);
    bool isOperator(char c);
    
  public:
    explicit Lexer(std::string filename);
    Token getNextToken();
};
