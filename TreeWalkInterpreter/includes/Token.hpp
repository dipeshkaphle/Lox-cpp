#pragma once

#include "TokenTypes.hpp"
#include <any>
#include <string>

using namespace std;

class Token {
private:
  string literal_to_string() const {
    switch (type) {
    case TokenType::STRING:
      return std::any_cast<string>(literal);
    case TokenType::NUMBER:
      return std::to_string(std::any_cast<double>(literal));
    default:
      return "";
    }
  }

public:
  TokenType type;
  string lexeme;
  std::any literal;
  int line;
  Token(TokenType type, const string &lexeme, const std::any &literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}
  string to_string() {
    return type_to_string(type) + " " + lexeme + " " + literal_to_string();
  }
};

