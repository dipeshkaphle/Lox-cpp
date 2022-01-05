#include "includes/Expr/binary_expr.hpp"

using namespace std;

binary_expr::binary_expr(Token _op, unique_ptr<Expr> left,
                         unique_ptr<Expr> right)
    : left((std::move(left))), right((std::move(right))), op(move(_op)) {}

std::any binary_expr::accept(expr_visitor<std::any> &visitor) {
  return visitor.visit_binary_expr(*this);
}
