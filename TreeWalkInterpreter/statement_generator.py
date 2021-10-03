filename = 'WhileStmt.hpp'
class_name = 'while_stmt'

attributes = ["std::unique_ptr<Expr> condition",
              "std::unique_ptr<Stmt> body"]

members_list = '\n'.join(["  "+att+";" for att in attributes])
constructor_params = ','.join([att+"_" for att in attributes])
print(members_list)
print(constructor_params)

prog = '''
#pragma once

#include "../Expr/Expr.hpp"
#include "../Token.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>

class {} : public Stmt {}
public :
{}

  {} ({});
  std::any accept(stmt_visitor<std::any> &visitor) override;
{};'''.format(class_name, '{', members_list, class_name, constructor_params, '}')

open("includes/Stmt/{}".format(filename), "w").write(prog)
