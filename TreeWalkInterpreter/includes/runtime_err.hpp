#pragma once

#include "Token.hpp"
#include <stdexcept>

struct Lox_runtime_err : public std::runtime_error {
  Token token;
  explicit Lox_runtime_err(Token tok, const char *err_msg)
      : token(std::move(tok)), runtime_error(err_msg) {}
};
