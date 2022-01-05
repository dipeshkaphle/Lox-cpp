#include "includes/Expr/assign_expr.hpp"

std::any assign_expr::accept(expr_visitor<std::any> &visitor) {
  return visitor.visit_assign_expr(*this);
}
