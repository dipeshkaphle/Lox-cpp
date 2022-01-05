#include "includes/Expr/grouping_expr.hpp"
using namespace std;

std::any grouping_expr::accept(expr_visitor<std::any> &visitor) {
  return visitor.visit_grouping_expr(*this);
}
