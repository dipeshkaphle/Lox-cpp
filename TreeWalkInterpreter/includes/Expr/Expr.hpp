#pragma once
#include <any>

#include "ExprVisitor.hpp"

class Expr {
public:
  virtual std::any accept(const expr_visitor<std::any> &visitor) const = 0;
};
