#pragma once
#include <any>

#include "ExprVisitor.hpp"

class Expr {
public:
  virtual ~Expr() = default;
  virtual std::any accept(const expr_visitor<std::any> &visitor) const = 0;
};
