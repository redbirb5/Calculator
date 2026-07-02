#pragma once

#include "calculation_types.h"

#include <vector>

namespace app
{

class CalculationStorage
{
  public:
    void initialize();
    std::vector<CalculationRecord> loadAll() const;
    void save(const CalculationRecord& record);
};

} // namespace app