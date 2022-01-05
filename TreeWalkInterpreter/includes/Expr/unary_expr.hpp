#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

struct unary_expr : public Expr {
  std::unique_ptr<Expr> right;
  Token op;

  unary_expr(Token _op, unique_ptr<Expr> right);

  std::any accept(expr_visitor<std::any> &visitor) override;
} __attribute__((aligned(128)));
