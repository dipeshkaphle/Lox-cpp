#include "includes/Expr/call_expr.hpp"

call_expr::call_expr(std::unique_ptr<Expr> callee, Token paren,
                     vector<unique_ptr<Expr>> arguments)
    : callee(move(callee)), paren(move(paren)), arguments(move(arguments)) {}

std::any call_expr::accept(expr_visitor<std::any> &visitor) {
  return visitor.visit_call_expr(*this);
}
