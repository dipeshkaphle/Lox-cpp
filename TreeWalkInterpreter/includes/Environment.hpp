#pragma once

#include <any>
#include <string>
#include <unordered_map>
#include <vector>

#include "Token.hpp"
#include "runtime_err.hpp"

#include "tl/expected.hpp"
using namespace tl;

using any_or_err = tl::expected<std::any, Lox_runtime_err>;
using map_type = std::unordered_map<std::string, std::any>;

class Environment {
private:
  // dont make it mutable lol
  mutable std::vector<map_type> sym_table;

public:
  Environment() : sym_table(1, map_type()) {}
  any_or_err get(const Token &name) const;
  void define(const std::string &name, std::any val);

  // because the sym_table is mutable, this can be const despite the fact that
  // ill be mutating sym_table
  any_or_err assign(const Token &name, std::any val) const;
};
