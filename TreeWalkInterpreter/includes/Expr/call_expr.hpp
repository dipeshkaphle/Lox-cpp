#pragma once

#include <memory>

#include "../Token.hpp"
#include "Expr.hpp"
#include "ExprVisitor.hpp"

using namespace std;
struct call_expr : public Expr {
  std::unique_ptr<Expr> callee;
  Token paren;
  vector<unique_ptr<Expr>> arguments;
  call_expr(std::unique_ptr<Expr> callee, Token paren,
            vector<unique_ptr<Expr>> arguments);

  std::any accept(expr_visitor<std::any> &visitor) override;
};
