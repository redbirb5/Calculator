#pragma once

#include "calculation_types.h"

#include <memory>

namespace app
{

class Calculator
{
  public:
    Calculator();
    ~Calculator();

    int calculate(const Request& request) const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace app