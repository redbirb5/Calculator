#include "calculation_cache.h"

namespace app
{
std::optional<CalculationRecord>
    CalculationCache::find(const Request& req) const
{
    auto it = cache_map_.find(makeKey(req));
    if (it != cache_map_.end())
        return it->second;

    return std::nullopt;
}

void CalculationCache::add(CalculationRecord rec)
{
    cache_map_.insert_or_assign(makeKey(rec.request), rec);
}

std::string CalculationCache::makeKey(const Request& req) const
{
    if (req.operation == Operation::Add || req.operation == Operation::Multiply)
    {
        if (req.value1 > req.value2.value())
        {
            return std::to_string(req.value2.value()) +
                   getOperationSymbol(req.operation) +
                   std::to_string(req.value1);
        }
    }
    if (req.operation == Operation::Factorial)
    {
        return std::to_string(req.value1) + getOperationSymbol(req.operation);
    }
    return std::to_string(req.value1) + getOperationSymbol(req.operation) +
           std::to_string(req.value2.value());
}

} // namespace app
