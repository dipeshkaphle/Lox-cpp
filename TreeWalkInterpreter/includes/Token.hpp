#ifndef TOKENHPP
#define TOKENHPP
#include "TokenTypes.hpp"
#include <string>

using namespace std;

class Token {
private:
  TokenType type;
  string lexeme;
  string literal;
  int line;

public:
  Token(TokenType type, const string &lexeme, const string &literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}
  string to_string() {
    return type_to_string(type) + " " + lexeme + " " + literal;
  }
};

#endif
