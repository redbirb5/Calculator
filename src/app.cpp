#include "app.h"

#include "libmath.h"

#include <nlohmann/json.hpp>

#include <iostream>
#include <memory>
#include <stdexcept>

namespace app
{

Request JsonParser::parse(int argc, char** argv) const
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
    req.operation = recognizeOperation(data.at("operation").get<std::string>());
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

Operation JsonParser::recognizeOperation(const std::string& operation) const
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

class Calculator::Impl
{
  public:
    libmath::Arithmetic math_;
};

Calculator::Calculator() : impl_(std::make_unique<Impl>())
{}

Calculator::~Calculator() = default;

int Calculator::calculate(const Request& req) const
{
    switch (req.operation)
    {
        case Operation::Add:
            return impl_->math_.add(req.value1, req.value2.value());
        case Operation::Subtract:
            return impl_->math_.subtract(req.value1, req.value2.value());
        case Operation::Multiply:
            return impl_->math_.multiply(req.value1, req.value2.value());
        case Operation::Divide:
            return impl_->math_.divide(req.value1, req.value2.value());
        case Operation::Power:
            return impl_->math_.power(req.value1, req.value2.value());
        case Operation::Factorial:
            return impl_->math_.factorial(req.value1);
        default:
            throw std::invalid_argument("Unknown operation");
    }
}

void Printer::print(const Request& req, int result) const
{
    if (req.operation == Operation::Factorial)
    {
        std::cout << req.value1 << getOperationSymbol(req.operation) << " = "
                  << result << "\n";
    }
    else
    {
        std::cout << req.value1 << " " << getOperationSymbol(req.operation)
                  << " " << req.value2.value() << " = " << result << "\n";
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
        << "  calculator '{\"operation\":\"subtract\",\"value1\":-5,\"value2\":-2}' -> -5 - -2 = -3\n"
        << "  calculator '{\"operation\":\"multiply\",\"value1\":3,\"value2\":4}' -> 3 * 4 = 12\n"
        << "  calculator '{\"operation\":\"divide\",\"value1\":10,\"value2\":2}' -> 10 / 2 = 5\n"
        << "  calculator '{\"operation\":\"power\",\"value1\":2,\"value2\":3}' -> 2 ^ 3 = 8\n"
        << "  calculator '{\"operation\":\"factorial\",\"value1\":3}' -> 3! = 6\n";
}

void Printer::printError(const std::string& error_message) const
{
    std::cerr << "Error! " << error_message << "\n";
}

std::string Printer::getOperationSymbol(Operation oprt) const
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
    try
    {
        Request req = json_parser_.parse(argc, argv);
        int result = calculator_.calculate(req);
        printer_.print(req, result);
    }
    catch (const std::exception& error)
    {
        printer_.printError(error.what());
        return 1;
    }
    return 0;
}

} // namespace app
