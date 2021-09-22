#ifndef CPPLOXHPP
#define CPPLOXHPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Token.hpp"

using std::string;
using std::string_view;

class Lox {
public:
  static bool hadError;
  static void error(int line, string_view message);
  static void report(int line, string_view where, string_view message);
  static void error(Token tok, const char *message);
};

#endif
