#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

struct literal_expr : public Expr {
  std::any value;

  literal_expr(std::any &&val) : value(move(val)) {}

  std::any accept(const expr_visitor<std::any> &visitor) const;
};
