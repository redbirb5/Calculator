#include "app.h"

#include "logger.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>

namespace app
{

Request Parser::parse(int argc, char** argv) const
{
    using json = nlohmann::json;

    if (argc != 2)
    {
        throw std::invalid_argument("Expected one JSON argument");
    }
    json data = json::parse(argv[1]);

    if (!data.contains("operation") || !data.contains("value1"))
    {
        throw std::invalid_argument(
            "Missing required JSON fields: operation or value1");
    }
    Request req;
    req.operation =
        operationFromString(data.at("operation").get<std::string>());
    if (req.operation == Operation::Unknown)
        throw std::invalid_argument("Unknown operation");
    req.value1 = data.at("value1").get<int>();

    if (req.operation != Operation::Factorial)
    {
        if (!data.contains("value2"))
            throw std::invalid_argument("Missing required JSON fields: value2");
        req.value2 = data.at("value2").get<int>();
    }

    return req;
}

void Printer::print(const CalculationRecord& rec) const
{
    if (rec.request.operation == Operation::Factorial)
    {
        std::cout << rec.request.value1
                  << getOperationSymbol(rec.request.operation) << " = "
                  << rec.result.value() << "\n";
    }
    else
    {
        std::cout << formatOperand(rec.request.value1) << " "
                  << getOperationSymbol(rec.request.operation) << " "
                  << formatOperand(rec.request.value2.value()) << " = "
                  << rec.result.value() << "\n";
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

bool CalculatorApp::isHelpRequested(int argc, char** argv) const
{
#pragma unroll 3
    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
        {
            return true;
        }
    }
    return false;
}

int CalculatorApp::run(int argc, char** argv)
{
    if (isHelpRequested(argc, argv))
    {
        printer_.printHelp();
        return 0;
    }

    if (argc == 2)
        Logger::instance().info(std::string("Calculation request received: ") +
                                argv[1]);

    try
    {
        Request req = json_parser_.parse(argc, argv);
        calc_service_.initialize();
        CalculationRecord rec = calc_service_.executeCalculation(req);
        if (rec.status == CalculationStatus::Success)
        {
            Logger::instance().info("Calculation result: " +
                                    std::to_string(rec.result.value()));
            printer_.print(rec);
        }
        else
        {
            Logger::instance().error("Calculation status: " +
                                     messageFromCalculationStatus(rec.status));
            printer_.printError(messageFromCalculationStatus(rec.status));
            return 1;
        }
    }
    catch (const std::exception& error)
    {
        Logger::instance().error(error.what());

        printer_.printError(error.what());
        return 1;
    }

    Logger::instance().info("Calculation completed successfully");

    return 0;
}

} // namespace app
