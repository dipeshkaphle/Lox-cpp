#pragma once

#include "Expr/ExprVisitor.hpp"
#include "Stmt/Stmt.hpp"
#include "Stmt/StmtVisitor.hpp"
#include "includes/Expr/Expr.hpp"
#include "includes/Token.hpp"

#include <any>
#include <array>
#include <memory>

class interpreter : public expr_visitor<std::any>, stmt_visitor<std::any> {
private:
  std::any visit_binary_expr(const binary_expr &exp) const override;
  std::any visit_unary_expr(const unary_expr &exp) const override;
  std::any visit_grouping_expr(const grouping_expr &exp) const override;
  std::any visit_literal_expr(const literal_expr &exp) const override;

  std::any evaluate(const Expr &exp) const;
  std::any execute(const Stmt &stmt) const;

  std::any visit_print_stmt(const print_stmt &stmt) const override;
  std::any visit_expr_stmt(const expr_stmt &stmt) const override;

  static bool is_truthy(const std::any &val);
  static bool is_equal(const std::any &l, const std::any &r);
  static string stringify(const std::any &obj);

  template <std::size_t N>
  static void
  check_number_operands(const Token &tok,
                        std::array<std::reference_wrapper<std::any>, N>);

public:
  void interpret(vector<std::unique_ptr<Stmt>> &stmts) const;
};
