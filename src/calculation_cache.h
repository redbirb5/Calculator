#pragma once

#include "calculation_types.h"

#include <string>
#include <unordered_map>

namespace app
{

class CalculationCache
{
  public:
    std::optional<CalculationRecord> find(const Request& request) const;
    void add(CalculationRecord record);

  private:
    std::string makeKey(const Request& request) const;
    std::unordered_map<std::string, CalculationRecord> cache_map_;
};

} // namespace app