#ifndef CPPLOXCPP
#define CPPLOXCPP

#include "CppLox.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Scanner.hpp"

using std::string;
using std::string_view;

void CppLox::run(const string &source) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();
  for (Token token : tokens) {
    std::cout << token.to_string() << '\n';
  }
}

void CppLox::runFile(char *filename) {
  std::ifstream inp(filename);

  if (inp.is_open()) {

    string all_code;
    string tmp;
    while (std::getline(inp, tmp)) {
      all_code.append(tmp);
      all_code.push_back('\n');
    }
    CppLox lox;
    lox.run(all_code);
    if (CppLox::hadError) {
      exit(20);
    }
  } else {
    throw std::runtime_error("Couldnt open the file " + string(filename));
  }
}

void CppLox::runPrompt() {
  CppLox::hadError = false;
  CppLox lox;
  string line;
  while (true) {
    std::getline(std::cin, line);
    if (line == "") {
      break;
    } else {
      lox.run(line);
      CppLox::hadError = false;
    }
  }
}

#endif
