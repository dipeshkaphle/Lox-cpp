#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

struct binary_expr : public Expr {
  std::unique_ptr<Expr> left, right;
  Token op;

  binary_expr(Token _op, unique_ptr<Expr> left, unique_ptr<Expr> right);

  std::any accept(const expr_visitor<std::any> &visitor) const override;
} __attribute__((aligned(128)));
