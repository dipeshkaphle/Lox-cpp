#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

struct assign_expr : public Expr {
  Token name;
  std::unique_ptr<Expr> value;

  assign_expr(Token _name, std::unique_ptr<Expr> exp)
      : name(move(_name)), value(std::move(exp)) {}

  std::any accept(expr_visitor<std::any> &visitor) override;
};
