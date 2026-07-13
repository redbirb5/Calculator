#include "calculation_cache.h"

namespace app
{
std::optional<CalculationRecord>
    CalculationCache::find(const Request& request) const
{
    auto it = cache_map_.find(makeKey(request));
    if (it != cache_map_.end())
        return it->second;

    return std::nullopt;
}

void CalculationCache::add(CalculationRecord record)
{
    cache_map_.insert_or_assign(makeKey(record.request), record);
}

std::string CalculationCache::makeKey(const Request& request) const
{
    if (request.operation == Operation::Add ||
        request.operation == Operation::Multiply)
    {
        if (request.value1 > request.value2.value())
        {
            return std::to_string(request.value2.value()) +
                   getOperationSymbol(request.operation) +
                   std::to_string(request.value1);
        }
    }
    if (request.operation == Operation::Factorial)
    {
        return std::to_string(request.value1) +
               getOperationSymbol(request.operation);
    }
    return std::to_string(request.value1) +
           getOperationSymbol(request.operation) +
           std::to_string(request.value2.value());
}

} // namespace app
