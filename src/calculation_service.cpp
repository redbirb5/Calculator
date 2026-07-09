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

    for (const auto& rec : records)
    {
        cache_.add(rec);
    }
}

CalculationRecord CalculationService::executeCalculation(const Request& req)
{
    std::optional<CalculationRecord> cached_rec = cache_.find(req);
    if (cached_rec)
    {
        return cached_rec.value();
    }

    CalculationRecord rec;
    rec.request = req;

    try
    {
        rec.result = calculator_.calculate(req);
        rec.status = CalculationStatus::Success;
    }
    catch (const std::exception& err)
    {
        rec.result = std::nullopt;
        rec.status = calculationStatusFromLibMathException(err);
    }

    cache_.add(rec);
    storage_.save(rec);

    return rec;
}

CalculationStatus CalculationService::calculationStatusFromLibMathException(
    const std::exception& err)
{
    const std::string msg = err.what();

    if (msg == "Division by zero")
        return CalculationStatus::DivisionByZero;
    if (msg == "Raising to a negative power")
        return CalculationStatus::NegativePower;
    if (msg == "Factorial of a negative number")
        return CalculationStatus::NegativeFactorial;
    if (msg == "Type overflow")
        return CalculationStatus::TypeOverflow;

    return CalculationStatus::UnknownCalculationError;
}

} // namespace app
