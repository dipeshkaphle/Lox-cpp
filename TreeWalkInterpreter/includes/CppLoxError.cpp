#ifndef CPPLOXERRORCPP
#define CPPLOXERRORCPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "CppLox.hpp"
#include "Scanner.hpp"
#include "Token.hpp"
#include "TokenTypes.hpp"

using std::string;
using std::string_view;

void CppLox::error(int line, string_view message) {
  CppLox::report(line, "", message);
}

void CppLox::report(int line, string_view where, string_view message) {
  std::cerr << "[line " << line << "] Error " << where << ": " << message;
  CppLox::hadError = true;
}
#endif
