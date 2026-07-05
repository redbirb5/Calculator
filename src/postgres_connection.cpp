#include "postgres_connection.h"

#include <libpq-fe.h>

#include <stdexcept>

namespace app
{
class PostgresConnection::Impl
{
  public:
    explicit Impl(const std::string& connection_string) :
        connection_(PQconnectdb(connection_string.c_str()), &PQfinish)
    {
        if (PQstatus(connection_.get()) != CONNECTION_OK)
        {
            throw std::runtime_error(PQerrorMessage(connection_.get()));
        }
    }
    std::unique_ptr<PGconn, decltype(&PQfinish)> connection_;
};

PostgresConnection::PostgresConnection(const std::string& connection_string) :
    impl_(std::make_unique<Impl>(connection_string))
{}

PostgresConnection::~PostgresConnection() = default;

} // namespace app
