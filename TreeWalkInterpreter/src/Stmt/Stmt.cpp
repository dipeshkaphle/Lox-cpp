#include "includes/Stmt/Stmt.hpp"
#include "includes/Stmt/ExprStmt.hpp"
#include "includes/Stmt/PrintStmt.hpp"

/*
 * =============================================================================
 * Expression Statement
 */

expr_stmt::expr_stmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

std::any expr_stmt::accept(const stmt_visitor<std::any> &visitor) const {
  return visitor.visit_expr_stmt(*this);
}

/*
 * Expression Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * Print Statement
 */

print_stmt::print_stmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

std::any print_stmt::accept(const stmt_visitor<std::any> &visitor) const {
  return visitor.visit_print_stmt(*this);
}

/*
 * Print Statement End
 *  =============================================================================
 */
