#pragma once

#include <any>
#include <vector>

class interpreter;

class Callable {
public:
  virtual size_t arity() = 0;
  virtual std::any call([[maybe_unused]] interpreter &intrptr,
                        [[maybe_unused]] std::vector<std::any> &arguments) = 0;
};
