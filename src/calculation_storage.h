#pragma once

#include "calculation_types.h"
#include "postgres_connection.h"

#include <string>
#include <vector>

namespace app
{

class CalculationStorage
{
  public:
    explicit CalculationStorage(const std::string& connection_string);

    void initialize();
    std::vector<CalculationRecord> loadAll() const;
    void save(const CalculationRecord& rec);

  private:
    PostgresConnection p_connection_;
};

} // namespace app