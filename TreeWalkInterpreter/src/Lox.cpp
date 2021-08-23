#include "Lox.hpp"

using std::string;
using std::string_view;

bool Lox::hadError = false;

void Lox::error(int line, string_view message) {
  Lox::report(line, "", message);
}

void Lox::report(int line, string_view where, string_view message) {
  std::cerr << "[line " << line << "] Error " << where << ": " << message;
  Lox::hadError = true;
}
