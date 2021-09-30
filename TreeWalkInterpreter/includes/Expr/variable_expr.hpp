#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

struct variable_expr : public Expr {
  Token name;

  variable_expr(Token _name) : name(move(_name)) {}

  std::any accept(const expr_visitor<std::any> &visitor) const override;
};
