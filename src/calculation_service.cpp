#include "calculation_service.h"

namespace app
{
CalculationService::CalculationService(std::string connection_string) :
    storage_(connection_string)
{}

void CalculationService::initialize()
{
    storage_.initialize();
    auto records = storage_.loadAll();

    for (const auto& record : records)
    {
        cache_.add(record);
    }
}

CalculationRecord CalculationService::executeCalculation(const Request& request)
{
    std::optional<CalculationRecord> cached_record = cache_.find(request);
    if (cached_record)
    {
        return cached_record.value();
    }

    CalculationRecord record;
    record.request = request;

    try
    {
        record.result = calculator_.calculate(request);
        record.status = CalculationStatus::Success;
    }
    catch (const std::exception& error)
    {
        record.result = std::nullopt;
        record.status = calculationStatusFromLibMathException(error);
    }

    storage_.save(record);
    cache_.add(record);

    return record;
}

CalculationStatus CalculationService::calculationStatusFromLibMathException(
    const std::exception& error)
{
    const std::string message = error.what();

    if (message == "Division by zero")
        return CalculationStatus::DivisionByZero;
    if (message == "Raising to a negative power")
        return CalculationStatus::NegativePower;
    if (message == "Factorial of a negative number")
        return CalculationStatus::NegativeFactorial;
    if (message == "Type overflow")
        return CalculationStatus::TypeOverflow;

    return CalculationStatus::UnknownCalculationError;
}

} // namespace app
