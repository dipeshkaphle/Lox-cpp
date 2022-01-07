#pragma once

#include "includes/Callable.hpp"
#include "includes/Stmt/FnStmt.hpp"

#include <memory>

class LoxFn : public Callable {
private:
  fn_stmt fn_decl;

public:
  explicit LoxFn(fn_stmt fn);

  size_t arity() final;

  std::any call([[maybe_unused]] interpreter &intrptr,
                [[maybe_unused]] std::vector<std::any> &arguments) final;
  std::string to_string() const final;
};
