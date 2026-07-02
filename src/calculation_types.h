#pragma once

#include <optional>
#include <string>

namespace app
{

enum class Operation
{
    Unknown = 0,
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,
    Factorial,
};

struct Request
{
    Operation operation;
    int value1;
    std::optional<int> value2;
};

enum class CalculationStatus
{
    Success = 0,
    DivisionByZero = 1,
    NegativePower = 2,
    NegativeFactorial = 3,
    TypeOverflow = 4,
    UnknownCalculationError = 5,
};

struct CalculationRecord
{
    Request request;
    std::optional<int> result;
    CalculationStatus status;
    std::string error_message;
};

} // namespace app