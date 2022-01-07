#include "includes/AstPrinter.hpp"

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
  return fmt::format("PRINT {}", this->print(*stmt.expr));
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
      "IF {} \nTHEN \n{} \nELSE\n {}\n ENDIF", this->print(*stmt.condition),
      this->print(*stmt.then_branch),
      stmt.else_branch.has_value() ? this->print(*stmt.else_branch.value())
                                   : "EMPTY"s);
}

std::any ast_printer::visit_while_stmt(while_stmt &stmt) {
  return fmt::format("WHILE {} <BLOCK> ENDWHILE", this->print(*stmt.condition));
}

std::any ast_printer::visit_break_stmt([[maybe_unused]] break_stmt &stmt) {
  return "BREAK"s;
}

std::any
ast_printer::visit_continue_stmt([[maybe_unused]] continue_stmt &stmt) {
  return "CONTINUE"s;
}

std::any ast_printer::visit_fn_stmt(fn_stmt &stmt) {
  return fmt::format("DECL FN {} ENDFN", stmt.name.lexeme);
}
