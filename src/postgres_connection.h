#pragma once

#include <memory>
#include <string>

struct pg_result;
using PGresult = pg_result;

namespace app
{

class PostgresResult
{
  public:
    explicit PostgresResult(PGresult* result);
    ~PostgresResult();

    int rowsCount() const;
    std::string value(int row, int column) const;
    bool isNull(int row, int column) const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

class PostgresConnection
{
  public:
    explicit PostgresConnection(const std::string& connection_string);
    ~PostgresConnection();

    void executeCommand(const std::string& command) const;
    PostgresResult executeQuery(const std::string& query) const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace app