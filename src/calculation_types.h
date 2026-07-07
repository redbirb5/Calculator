#pragma once

#include <optional>
#include <stdexcept>
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

inline Operation operationFromString(const std::string& operation)
{
    if (operation == "add")
    {
        return Operation::Add;
    }
    else if (operation == "subtract")
    {
        return Operation::Subtract;
    }
    else if (operation == "multiply")
    {
        return Operation::Multiply;
    }
    else if (operation == "divide")
    {
        return Operation::Divide;
    }
    else if (operation == "power")
    {
        return Operation::Power;
    }
    else if (operation == "factorial")
    {
        return Operation::Factorial;
    }
    else
    {
        return Operation::Unknown;
    }
}

inline std::string operationToString(Operation operation)
{
    switch (operation)
    {
        case Operation::Add:
            return "add";
        case Operation::Subtract:
            return "subtract";
        case Operation::Multiply:
            return "multiply";
        case Operation::Divide:
            return "divide";
        case Operation::Power:
            return "power";
        case Operation::Factorial:
            return "factorial";
        default:
            return "unknown";
    }
}

inline CalculationStatus calculationStatusFromString(const std::string& status)
{
    if (status == "0")
    {
        return CalculationStatus::Success;
    }
    else if (status == "1")
    {
        return CalculationStatus::DivisionByZero;
    }
    else if (status == "2")
    {
        return CalculationStatus::NegativePower;
    }
    else if (status == "3")
    {
        return CalculationStatus::NegativeFactorial;
    }
    else if (status == "4")
    {
        return CalculationStatus::TypeOverflow;
    }
    else
    {
        return CalculationStatus::UnknownCalculationError;
    }
}

inline std::string calculationStatusToString(CalculationStatus status)
{
    switch (status)
    {
        case CalculationStatus::Success:
            return "0";
        case CalculationStatus::DivisionByZero:
            return "1";
        case CalculationStatus::NegativePower:
            return "2";
        case CalculationStatus::NegativeFactorial:
            return "3";
        case CalculationStatus::TypeOverflow:
            return "4";
        default:
            return "unknown";
    }
}

inline std::string getOperationSymbol(Operation oprt)
{
    switch (oprt)
    {
        case Operation::Add:
            return "+";
        case Operation::Subtract:
            return "-";
        case Operation::Multiply:
            return "*";
        case Operation::Divide:
            return "/";
        case Operation::Power:
            return "^";
        case Operation::Factorial:
            return "!";
        default:
            throw std::invalid_argument("Unknown operation");
    }
}

} // namespace app