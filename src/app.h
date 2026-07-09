#pragma once

#include "calculation_service.h"
#include "calculation_types.h"
#include "calculator.h"

#include <memory>
#include <optional>
#include <string>

namespace app
{

class Parser
{
  public:
    Request parse(int argc, char** argv) const;
};

class Printer
{
  public:
    void print(const CalculationRecord& rec) const;
    void printHelp() const;
    void printError(const std::string& error_message) const;

  private:
    std::string formatOperand(int operand) const;
};

class CalculatorApp
{
  public:
    CalculatorApp(std::string connection_string) :
        calc_service_(connection_string)
    {}
    int run(int argc, char** argv);

  private:
    bool isHelpRequested(int argc, char** argv) const;
    Parser json_parser_;
    CalculationService calc_service_;
    Printer printer_;
};

} // namespace app
