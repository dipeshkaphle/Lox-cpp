#pragma once

#include "Environment.hpp"
#include "Expr/ExprVisitor.hpp"
#include "Stmt/Stmt.hpp"
#include "Stmt/StmtVisitor.hpp"
#include "includes/AstPrinter.hpp"
#include "includes/Expr/Expr.hpp"
#include "includes/Token.hpp"
#include "includes/globals.hpp"

#include <any>
#include <array>
#include <memory>

class interpreter : public expr_visitor<std::any>, stmt_visitor<std::any> {
private:
  std::any visit_binary_expr(binary_expr &exp) final;
  std::any visit_unary_expr(unary_expr &exp) final;
  std::any visit_grouping_expr(grouping_expr &exp) final;
  std::any visit_literal_expr(literal_expr &exp) final;
  std::any visit_variable_expr(variable_expr &exp) final;
  std::any visit_assign_expr(assign_expr &exp) final;
  std::any visit_logical_expr(logical_expr &exp) final;
  std::any visit_call_expr(call_expr &exp) final;

  std::any visit_print_stmt(print_stmt &stmt) final;
  std::any visit_expr_stmt(expr_stmt &stmt) final;
  std::any visit_let_stmt(let_stmt &stmt) final;
  std::any visit_block_stmt(block_stmt &stmt) final;
  std::any visit_if_stmt(if_stmt &stmt) final;
  std::any visit_while_stmt(while_stmt &stmt) final;
  std::any visit_break_stmt(break_stmt &stmt) final;
  std::any visit_continue_stmt(continue_stmt &stmt) final;
  std::any visit_fn_stmt(fn_stmt &stmt) final;
  std::any visit_return_stmt(return_stmt &stmt) final;

  static bool is_truthy(const std::any &val);
  static bool is_equal(const std::any &l, const std::any &r);
  static string stringify(const std::any &obj);

  template <std::size_t N>
  static void
  check_number_operands(const Token &tok,
                        std::array<std::reference_wrapper<std::any>, N>);

  Environment env{};
  bool break_from_current_loop{};
  bool continue_loop{};
  bool return_from_here{};

public:
  static inline ast_printer printer{};
  interpreter() { this->env = GlobalEnv::init(); }
  void interpret(vector<std::unique_ptr<Stmt>> &stmts, bool is_repl = false);
  Environment &get_env();
  std::any evaluate(Expr &exp);
  std::any execute(Stmt &stmt);
  std::any execute_block(vector<std::unique_ptr<Stmt>> &stmts);
};
