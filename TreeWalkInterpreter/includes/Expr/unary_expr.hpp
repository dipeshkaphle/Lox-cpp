#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

class unary_expr : public Expr {
private:
  std::unique_ptr<Expr> right;
  Token op;

public:
  unary_expr(Token _op, unique_ptr<Expr> right);

  std::any accept(const expr_visitor<std::any> &visitor) const;
};
