#pragma once

#include "calculation_cache.h"
#include "calculation_storage.h"
#include "calculation_types.h"
#include "calculator.h"

#include <optional>

namespace app
{

class CalculationService
{
  public:
    CalculationService(std::string connection_string);

    CalculationRecord executeCalculation(Request& req);

  private:
    CalculationStatus
        calculationStatusFromLibMathException(const std::exception& error);

    CalculationCache cache_;
    Calculator calculator_;
    CalculationStorage storage_;
};

} // namespace app
