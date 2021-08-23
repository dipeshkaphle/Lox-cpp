#include "Expr/literal_expr.hpp"

std::any literal_expr::accept(const expr_visitor<std::any> &visitor) const {
  return visitor.visit_literal_expr(*this);
}
