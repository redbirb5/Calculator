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
    std::optional<CalculationRecord> execute(Request& req);

  private:
    CalculationCache cache_;
    Calculator calculator_;
    CalculationStorage storage_;
};

} // namespace app
