#pragma once

#include "calculation_types.h"

namespace app
{

class Printer
{
  public:
    void print(const CalculationRecord& record) const;
    void printHelp() const;
    void printError(const std::string& error_message) const;

  private:
    std::string formatOperand(int operand) const;
};

} // namespace app
