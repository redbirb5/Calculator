#pragma once

#include <memory>
#include <string>

namespace app 
{

class PostgresConnection 
{
    public:
    explicit PostgresConnection(const std::string& connection_string);
    ~PostgresConnection();

    void executeCommand(const std::string& command) const;

    private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}