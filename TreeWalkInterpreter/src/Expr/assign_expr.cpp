#include "includes/Expr/assign_expr.hpp"

std::any assign_expr::accept(const expr_visitor<std::any> &visitor) const {
  return visitor.visit_assign_expr(*this);
}
