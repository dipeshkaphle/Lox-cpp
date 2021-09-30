#include "includes/Expr/variable_expr.hpp"

std::any variable_expr::accept(const expr_visitor<std::any> &visitor) const {
  return visitor.visit_variable_expr(*this);
}
