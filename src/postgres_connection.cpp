#include "postgres_connection.h"

#include <libpq-fe.h>
#include <stdexcept>

namespace app 
{
class PostgresConnection::Impl
{
    public:
    explicit Impl(const std::string& connection_string) 
        : connection_(PQconnectdb(connection_string.c_str()), &PQfinish)
        {
            if(PQstatus(connection_.get()) != CONNECTION_OK)
            {
                throw std::runtime_error(PQerrorMessage(connection_.get()));
            }
        }
    
    void executeCommand(const std::string& command) const;
    std::unique_ptr<PGconn, decltype(&PQfinish)> connection_;
};

void PostgresConnection::Impl::executeCommand(const std::string& command) const
{
    std::unique_ptr<PGresult, decltype(&PQclear)> result (
        PQexec(connection_.get(), command.c_str()),
        &PQclear
    );

    if(!result)
    {
        throw std::runtime_error(PQerrorMessage(connection_.get()));
    }

    if(PQresultStatus(result.get()) != PGRES_COMMAND_OK)
    {
        throw std::runtime_error(PQresultErrorMessage(result.get()));
    }
}

PostgresConnection::PostgresConnection(const std::string& connection_string)
    : impl_(std::make_unique<Impl>(connection_string))
{}

PostgresConnection::~PostgresConnection() = default;

void PostgresConnection::executeCommand(const std::string& command) const
{
    impl_->executeCommand(command);
}

} // namespace app
