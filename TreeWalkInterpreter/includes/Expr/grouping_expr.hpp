#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

struct grouping_expr : public Expr {
  std::unique_ptr<Expr> expression;

  grouping_expr(unique_ptr<Expr> val) : expression(std::move(val)) {}

  std::any accept(expr_visitor<std::any> &visitor) override;
};
