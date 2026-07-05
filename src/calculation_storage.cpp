#include "calculation_storage.h"

namespace app
{

CalculationStorage::CalculationStorage(const std::string& connection_string) :
    p_connection_(connection_string)
{}

void CalculationStorage::initialize()
{
    p_connection_.executeCommand(R"(
        CREATE TABLE IF NOT EXISTS calculations (
            id SERIAL PRIMARY KEY,
            operation TEXT NOT NULL,
            value1 INTEGER NOT NULL,
            value2 INTEGER,
            result INTEGER,
            status INTEGER NOT NULL,
            error_message TEXT NOT NULL DEFAULT ''
        );
        )");
}

} // namespace app