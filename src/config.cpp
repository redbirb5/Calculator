#include "config.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>

namespace app
{

std::string readConnectionString(const std::string& config_path)
{
    std::ifstream config_file(config_path);

    if (!config_file.is_open())
    {
        throw std::runtime_error("Failed to open config file: " + config_path);
    }

    nlohmann::json config;
    config_file >> config;

    const auto& database = config.at("database");
    const std::string host = database.at("host").get<std::string>();
    const int port = database.at("port").get<int>();
    const std::string dbname = database.at("dbname").get<std::string>();
    const std::string user = database.at("user").get<std::string>();
    const std::string password = database.at("password").get<std::string>();

    return "host=" + host + " port=" + std::to_string(port) +
           " dbname=" + dbname + " user=" + user + " password=" + password;
}

} // namespace app
