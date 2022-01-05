
#include "includes/Expr/logical_expr.hpp"

using namespace std;

logical_expr::logical_expr(Token _op, unique_ptr<Expr> left,
                           unique_ptr<Expr> right)
    : left((std::move(left))), right((std::move(right))), op(move(_op)) {}

std::any logical_expr::accept(expr_visitor<std::any> &visitor) {
  return visitor.visit_logical_expr(*this);
}
