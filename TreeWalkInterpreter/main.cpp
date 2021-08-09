#include "Scanner.hpp"

int main(int argc, char **argv) {
  if (argc > 2) {
    std::cout << "Usage: cpplox [script]";
    exit(255);
  } else if (argc == 2) {
    CppLox::runFile(argv[1]);
  } else {
    CppLox::runPrompt();
  }
}
