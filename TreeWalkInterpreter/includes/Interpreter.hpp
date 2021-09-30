#pragma once

#include "Environment.hpp"
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
  std::any visit_binary_expr(const binary_expr &exp) const final;
  std::any visit_unary_expr(const unary_expr &exp) const final;
  std::any visit_grouping_expr(const grouping_expr &exp) const final;
  std::any visit_literal_expr(const literal_expr &exp) const final;
  std::any visit_variable_expr(const variable_expr &exp) const final;
  std::any visit_assign_expr(const assign_expr &exp) const final;

  std::any evaluate(const Expr &exp) const;
  std::any execute(Stmt &stmt);

  std::any visit_print_stmt(print_stmt &stmt) final;
  std::any visit_expr_stmt(expr_stmt &stmt) final;
  std::any visit_let_stmt(let_stmt &stmt) final;

  static bool is_truthy(const std::any &val);
  static bool is_equal(const std::any &l, const std::any &r);
  static string stringify(const std::any &obj);

  template <std::size_t N>
  static void
  check_number_operands(const Token &tok,
                        std::array<std::reference_wrapper<std::any>, N>);

  Environment env;

public:
  interpreter() : env() {}
  void interpret(vector<std::unique_ptr<Stmt>> &stmts);
};
