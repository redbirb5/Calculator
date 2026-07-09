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
        );
        )");
}

std::vector<CalculationRecord> CalculationStorage::loadAll() const
{
    PostgresResult result = p_connection_.executeQuery(R"(
        SELECT operation, value1, value2, result, status
        FROM calculations;
        )");

    int v_size = result.rowsCount();
    std::vector<CalculationRecord> records(v_size);

    for (int i = 0; i < v_size; i++)
    {
        Request req;
        req.operation = operationFromString(result.value(i, 0));
        req.value1 = std::stoi(result.value(i, 1));
        if (!result.isNull(i, 2))
        {
            req.value2 = std::stoi(result.value(i, 2));
        }

        CalculationRecord c_rec;
        c_rec.request = req;
        if (!result.isNull(i, 3))
        {
            c_rec.result = std::stoi(result.value(i, 3));
        }
        c_rec.status = calculationStatusFromString(result.value(i, 4));

        records[i] = c_rec;
    }

    return records;
}

void CalculationStorage::save(const CalculationRecord& rec)
{
    std::vector<std::optional<std::string>> params;
    params.reserve(6);

    params.push_back(operationToString(rec.request.operation));
    params.push_back(std::to_string(rec.request.value1));
    params.push_back(rec.request.value2
                         ? std::optional<std::string>(
                               std::to_string(rec.request.value2.value()))
                         : std::nullopt);
    params.push_back(rec.result ? std::optional<std::string>(
                                      std::to_string(rec.result.value()))
                                : std::nullopt);
    params.push_back(calculationStatusToString(rec.status));

    p_connection_.executeParams(R"(
        INSERT INTO calculations (
            operation,
            value1,
            value2,
            result,
            status,
        )
        VALUES ($1, $2, $3, $4, $5);
    )",
                                params);
}

} // namespace app
