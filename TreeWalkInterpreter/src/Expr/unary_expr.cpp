#include "includes/Expr/unary_expr.hpp"
using namespace std;

unary_expr::unary_expr(Token _op, unique_ptr<Expr> right)
    : right(std::move(right)), op(move(_op)) {}

std::any unary_expr::accept(expr_visitor<std::any> &visitor) {
  return visitor.visit_unary_expr(*this);
}
