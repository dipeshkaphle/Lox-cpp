#include "CppLox.h"

void CppLox::run(const string &source) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scanTokens();
  for (Token token : tokens) {
    std::cout << token << '\n';
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
    run(all_code);
    if (CppLox::hadError) {
      exit(20);
    }
  } else {
    throw std::runtime_error("Couldnt open the file " + string(filename));
  }
}

void CppLox::runPrompt() {
  CppLox::hadError = false;
  string line;
  while (true) {
    std::getline(std::cin, line);
    if (line == "") {
      break;
    } else {
      run(line);
      CppLox::hadError = false;
    }
  }
}

void CppLox::error(int line, string_view message) { report(line, "", message); }

void CppLox::report(int line, string_view where, string_view message) {
  std::cerr << "[line " << line << "] Error " << where << ": " << message;
  CppLox::hadError = true;
}
