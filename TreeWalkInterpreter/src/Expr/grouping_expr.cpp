#include "includes/Expr/grouping_expr.hpp"
using namespace std;

std::any grouping_expr::accept(const expr_visitor<std::any> &visitor) const {
  return visitor.visit_grouping_expr(*this);
}
