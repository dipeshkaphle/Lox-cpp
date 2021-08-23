#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;

class literal_expr : public Expr {
private:
  std::any value;

public:
  literal_expr(std::any &&val) : value(move(val)) {}

  std::any accept(const expr_visitor<std::any> &visitor) const;
};
