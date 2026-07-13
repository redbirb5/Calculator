#include "printer.h"

#include <iostream>

namespace app
{

void Printer::print(const CalculationRecord& record) const
{
    if (record.request.operation == Operation::Factorial)
    {
        std::cout << record.request.value1
                  << getOperationSymbol(record.request.operation) << " = "
                  << record.result.value() << "\n";
    }
    else
    {
        std::cout << formatOperand(record.request.value1) << " "
                  << getOperationSymbol(record.request.operation) << " "
                  << formatOperand(record.request.value2.value()) << " = "
                  << record.result.value() << "\n";
    }
}

void Printer::printHelp() const
{
    std::cout
        << "Usage:\n"
        << "  calculator '<json>'\n"
        << "  calculator -h\n"
        << "  calculator --help\n\n"
        << "JSON format:\n"
        << "  {\"operation\":\"add\",\"value1\":1,\"value2\":2}\n"
        << "  {\"operation\":\"factorial\",\"value1\":5}\n\n"
        << "Fields:\n"
        << "  operation    add, subtract, multiply, divide, power, factorial\n"
        << "  value1       First integer value\n"
        << "  value2       Second integer value; required for all operations except factorial\n\n"
        << "Examples:\n"
        << "  calculator '{\"operation\":\"add\",\"value1\":1,\"value2\":2}' -> 1 + 2 = 3\n"
        << "  calculator '{\"operation\":\"subtract\",\"value1\":-5,\"value2\":-2}' -> (-5) - (-2) = -3\n"
        << "  calculator '{\"operation\":\"multiply\",\"value1\":3,\"value2\":4}' -> 3 * 4 = 12\n"
        << "  calculator '{\"operation\":\"divide\",\"value1\":10,\"value2\":2}' -> 10 / 2 = 5\n"
        << "  calculator '{\"operation\":\"power\",\"value1\":2,\"value2\":3}' -> 2 ^ 3 = 8\n"
        << "  calculator '{\"operation\":\"factorial\",\"value1\":3}' -> 3! = 6\n";
}

void Printer::printError(const std::string& error_message) const
{
    std::cerr << "Error! " << error_message << "\n";
}

std::string Printer::formatOperand(int operand) const
{
    if (operand < 0)
        return "(" + std::to_string(operand) + ")";
    return std::to_string(operand);
}

} // namespace app
