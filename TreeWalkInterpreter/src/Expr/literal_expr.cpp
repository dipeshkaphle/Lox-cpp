#include "includes/Expr/literal_expr.hpp"

std::any literal_expr::accept(expr_visitor<std::any> &visitor) {
  return visitor.visit_literal_expr(*this);
}
