#include "includes/Expr/variable_expr.hpp"

std::any variable_expr::accept(expr_visitor<std::any> &visitor) {
  return visitor.visit_variable_expr(*this);
}
