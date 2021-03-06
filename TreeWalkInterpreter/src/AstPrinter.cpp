#include "includes/AstPrinter.hpp"
#include "includes/Expr/assign_expr.hpp"
#include "includes/Expr/binary_expr.hpp"
#include "includes/Expr/call_expr.hpp"
#include "includes/Expr/grouping_expr.hpp"
#include "includes/Expr/literal_expr.hpp"
#include "includes/Expr/logical_expr.hpp"
#include "includes/Expr/unary_expr.hpp"
#include "includes/Expr/variable_expr.hpp"
#include "includes/Stmt/BlockStmt.hpp"
#include "includes/Stmt/BreakStmt.hpp"
#include "includes/Stmt/ContinueStmt.hpp"
#include "includes/Stmt/ExprStmt.hpp"
#include "includes/Stmt/FnStmt.hpp"
#include "includes/Stmt/IfStmt.hpp"
#include "includes/Stmt/LetStmt.hpp"
#include "includes/Stmt/PrintStmt.hpp"
#include "includes/Stmt/ReturnStmt.hpp"
#include "includes/Stmt/WhileStmt.hpp"

#include <fmt/core.h>

std::string ast_printer::parenthesize(std::string name, Expr &expr) {
  return fmt::format("( {} {} )", name, this->print(expr));
}

std::string ast_printer::parenthesize(std::string name, Expr &expr1,
                                      Expr &expr2) {
  return fmt::format("( {} {} {} )", name, this->print(expr1),
                     this->print(expr2));
}

std::string ast_printer::print(Expr &expr) {
  return std::any_cast<std::string>(expr.accept(*this));
}

std::any ast_printer::visit_binary_expr(binary_expr &expr) {
  return this->parenthesize(expr.op.lexeme, *expr.left, *expr.right);
}

std::any ast_printer::visit_unary_expr(unary_expr &expr) {
  return parenthesize(expr.op.lexeme, *expr.right);
}

std::any ast_printer::visit_grouping_expr(grouping_expr &expr) {
  return parenthesize("group", *expr.expression);
}

std::any ast_printer::visit_variable_expr(variable_expr &exp) {
  return exp.name.lexeme;
}

std::any ast_printer::visit_literal_expr(literal_expr &expr) {
  if (!expr.value.has_value())
    return std::string("nil");
  if (expr.value.type() == typeid(double)) {
    return to_string(any_cast<double>(expr.value));
  } else if (expr.value.type() == typeid(std::string)) {
    return any_cast<std::string>(expr.value);
  } else if (expr.value.type() == typeid(bool)) {
    if (any_cast<bool>(expr.value) == true)
      return std::string("true");
    else
      return std::string("false");
  }
  return std::string("nil");
}

std::any ast_printer::visit_assign_expr(assign_expr &exp) {
  return fmt::format("(ASSIGN {} {})", exp.name.lexeme,
                     this->print(*exp.value));
}

std::any ast_printer::visit_logical_expr(logical_expr &exp) {
  return parenthesize(exp.op.lexeme, *exp.left, *exp.right);
}

std::any ast_printer::visit_call_expr(call_expr &exp) {
  vector<string> vec;
  ranges::transform(exp.arguments, back_inserter(vec),
                    [&](auto &x) { return this->print(*x); });
  return fmt::format("( Call {} ( {} ) )", this->print(*exp.callee),
                     fmt::join(vec, ", "));
}

std::string ast_printer::print(Stmt &stmt) {
  return any_cast<string>(stmt.accept(*this));
}

std::any ast_printer::visit_print_stmt(print_stmt &stmt) {
  return fmt::format("PRINT{} {}", stmt.has_newline ? "LN" : "",
                     this->print(*stmt.expr));
}

std::any ast_printer::visit_expr_stmt(expr_stmt &stmt) {
  return fmt::format("{}", this->print(*stmt.expr));
}

std::any ast_printer::visit_let_stmt(let_stmt &stmt) {
  return fmt::format("LET {} {}", stmt.name.lexeme,
                     this->print(*stmt.initializer_expr));
}

std::any ast_printer::visit_block_stmt(block_stmt &stmt) {
  vector<string> stmts;
  ranges::transform(stmt.statements, back_inserter(stmts),
                    [&](auto &x) { return this->print(*x); });

  return fmt::format("BLOCK BEGIN \n {} \nBLOCK END", fmt::join(stmts, "\n"));
}

std::any ast_printer::visit_if_stmt(if_stmt &stmt) {
  return fmt::format(
      "IF {} \n  THEN \n{} \nELSE\n {}\nENDIF", this->print(*stmt.condition),
      this->print(*stmt.then_branch),
      stmt.else_branch.has_value() ? this->print(*stmt.else_branch.value())
                                   : "EMPTY"s);
}

std::any ast_printer::visit_while_stmt(while_stmt &stmt) {
  return fmt::format("WHILE {} \n\t <BLOCK> \n ENDWHILE",
                     this->print(*stmt.condition));
}

std::any ast_printer::visit_break_stmt([[maybe_unused]] break_stmt &stmt) {
  return "BREAK"s;
}

std::any
ast_printer::visit_continue_stmt([[maybe_unused]] continue_stmt &stmt) {
  return "CONTINUE"s;
}

std::any ast_printer::visit_fn_stmt(fn_stmt &stmt) {
  vector<string> args_name;
  ranges::transform(stmt.params, back_inserter(args_name),
                    [](auto &x) { return x.lexeme; });
  auto ret = fmt::format("DECL FN {} ( {} )\n", stmt.name.lexeme,
                         fmt::join(args_name, ", "));
  vector<string> printed_fn_body;
  ranges::transform(stmt.body, back_inserter(printed_fn_body),
                    [&](auto &x) { return "\t" + this->print(*x); });
  ret += fmt::format("{}\n", fmt::join(printed_fn_body, "\n"));
  ret += fmt::format("ENDFN\n");
  return ret;
}

std::any ast_printer::visit_return_stmt([[maybe_unused]] return_stmt &stmt) {
  return fmt::format("RETURN {}", stmt.value.has_value()
                                      ? this->print(*stmt.value.value())
                                      : "");
}
