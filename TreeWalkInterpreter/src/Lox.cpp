#include "includes/Lox.hpp"

#include "fmt/core.h"
#include "fmt/ostream.h"

using std::string;
using std::string_view;

bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;
interpreter Lox::interp = interpreter();

void Lox::error(int line, string_view message) {
  Lox::report(line, "", message);
}

void Lox::report_runtime_error(const Lox_runtime_err &err) {
  fmt::print(std::cerr, "{} [line {} ]\n", err.what(), err.token.line);
  hadRuntimeError = true;
}

void Lox::report(int line, string_view where, string_view message) {
  fmt::print(std::cerr, "[line {}] Error {} : {}\n", line, where, message);
  Lox::hadError = true;
}

void Lox::error(Token tok, const char *message) {
  if (tok.type == TokenType::ENDOFFILE) {
    report(tok.line, " at end", message);
  } else {
    report(tok.line, " at '"s + tok.lexeme + "'"s, message);
  }
}
