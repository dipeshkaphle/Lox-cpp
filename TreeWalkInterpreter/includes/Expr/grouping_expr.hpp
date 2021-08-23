#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

class grouping_expr : public Expr {
private:
  std::unique_ptr<Expr> expression;

public:
  grouping_expr(unique_ptr<Expr> val) : expression(std::move(val)) {}

  std::any accept(const expr_visitor<std::any> &visitor) const;
};
