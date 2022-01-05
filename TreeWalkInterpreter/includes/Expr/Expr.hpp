#pragma once
#include <any>

#include "ExprVisitor.hpp"

class Expr {
public:
  virtual ~Expr() = default;
  virtual std::any accept(expr_visitor<std::any> &visitor) = 0;
};
