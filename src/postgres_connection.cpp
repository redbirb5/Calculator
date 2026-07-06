#include "postgres_connection.h"

#include <libpq-fe.h>

#include <stdexcept>

namespace app
{

class PostgresResult::Impl
{
  public:
    explicit Impl(PGresult* result) : result_(result, &PQclear)
    {}

    int rowsCount() const;
    std::string value(int row, int column) const;
    bool isNull(int row, int column) const;

    std::unique_ptr<PGresult, decltype(&PQclear)> result_;
};

int PostgresResult::Impl::rowsCount() const
{
    return PQntuples(result_.get());
}

std::string PostgresResult::Impl::value(int row, int column) const
{
    return PQgetvalue(result_.get(), row, column);
}

bool PostgresResult::Impl::isNull(int row, int column) const
{
    return PQgetisnull(result_.get(), row, column);
}

PostgresResult::PostgresResult(PGresult* result) :
    impl_(std::make_unique<Impl>(result))
{}

PostgresResult::~PostgresResult() = default;

int PostgresResult::rowsCount() const
{
    return impl_->rowsCount();
}

std::string PostgresResult::value(int row, int column) const
{
    return impl_->value(row, column);
}

bool PostgresResult::isNull(int row, int column) const
{
    return impl_->isNull(row, column);
}

//////////////////////////////////////////////////////////////////////////

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

    void executeCommand(const std::string& command) const;
    PostgresResult executeQuery(const std::string& query) const;
    std::unique_ptr<PGconn, decltype(&PQfinish)> connection_;
};

void PostgresConnection::Impl::executeCommand(const std::string& command) const
{
    std::unique_ptr<PGresult, decltype(&PQclear)> result(
        PQexec(connection_.get(), command.c_str()), &PQclear);

    if (!result)
    {
        throw std::runtime_error(PQerrorMessage(connection_.get()));
    }

    if (PQresultStatus(result.get()) != PGRES_COMMAND_OK)
    {
        throw std::runtime_error(PQresultErrorMessage(result.get()));
    }
}

PostgresResult
    PostgresConnection::Impl::executeQuery(const std::string& query) const
{
    std::unique_ptr<PGresult, decltype(&PQclear)> result(
        PQexec(connection_.get(), query.c_str()), &PQclear);

    if (!result)
    {
        throw std::runtime_error(PQerrorMessage(connection_.get()));
    }

    if (PQresultStatus(result.get()) != PGRES_TUPLES_OK)
    {
        throw std::runtime_error(PQresultErrorMessage(result.get()));
    }
    return PostgresResult(result.release());
}

PostgresConnection::PostgresConnection(const std::string& connection_string) :
    impl_(std::make_unique<Impl>(connection_string))
{}

PostgresConnection::~PostgresConnection() = default;

void PostgresConnection::executeCommand(const std::string& command) const
{
    impl_->executeCommand(command);
}

PostgresResult PostgresConnection::executeQuery(const std::string& query) const
{
    return impl_->executeQuery(query);
}

} // namespace app
