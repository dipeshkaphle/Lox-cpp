#include "includes/Environment.hpp"

#include "fmt/core.h"
#include "tl/expected.hpp"

#include <ranges>

any_or_err Environment::get(const Token &name) const {
  for (const auto &table : this->sym_table | std::ranges::views::reverse) {
    if (table.contains(name.lexeme)) {
      return table.at(name.lexeme);
    }
  }
  std::string s;
  fmt::format_to(back_inserter(s), "Undefined variable: {}. ", name.lexeme);
  return tl::make_unexpected<Lox_runtime_err>(Lox_runtime_err(name, s.c_str()));
}

void Environment::define(const std::string &name, std::any val) {
  this->sym_table.back()[name] = std::move(val);
}

any_or_err Environment::assign(const Token &name, std::any val) const {
  for (auto &table : this->sym_table | std::ranges::views::reverse) {
    if (table.contains(name.lexeme)) {
      table[name.lexeme] = std::move(val);
      return table[name.lexeme];
    }
  }
  std::string s;
  fmt::format_to(back_inserter(s), "Undefined variable: {}. ", name.lexeme);
  return tl::make_unexpected<Lox_runtime_err>(Lox_runtime_err(name, s.c_str()));
}
