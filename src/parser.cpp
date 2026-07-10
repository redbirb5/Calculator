#include "parser.h"

#include <nlohmann/json.hpp>

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
    Request request;
    request.operation =
        operationFromString(data.at("operation").get<std::string>());
    if (request.operation == Operation::Unknown)
        throw std::invalid_argument("Unknown operation");
    request.value1 = data.at("value1").get<int>();

    if (request.operation != Operation::Factorial)
    {
        if (!data.contains("value2"))
            throw std::invalid_argument("Missing required JSON fields: value2");
        request.value2 = data.at("value2").get<int>();
    }

    return request;
}

} // namespace app
