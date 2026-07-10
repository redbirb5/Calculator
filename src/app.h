#pragma once

#include "calculation_service.h"
#include "calculation_types.h"
#include "parser.h"
#include "printer.h"

#include <string>

namespace app
{

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
