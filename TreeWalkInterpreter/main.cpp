#include "CppLox.h"
int main(int argc, char **argv) {
  if (argc > 2) {
    std::cout << "Usage: cpplox [script]";
    exit(255);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}
