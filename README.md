# calculator

CLI calculator in C++ using `libmath`, PostgreSQL storage, and an in-memory
cache for previously calculated operations.

Current version: `2.0.0`.

## Features

- parses calculation requests from JSON;
- supports `add`, `subtract`, `multiply`, `divide`, `power`, and `factorial`;
- stores calculation history in PostgreSQL;
- warms an `std::unordered_map` cache from the database on application start;
- reuses cached results for repeated operations;
- treats `1+2` and `2+1` as the same cache key for commutative operations;
- wraps `libpq` resources with RAII classes;
- writes application logs to `calculator.log`.

## Requirements

- CMake 3.10 or newer;
- C++ compiler;
- PostgreSQL server and client tools;
- PostgreSQL development package for `libpq`;
- Valgrind, if memory checks are needed;
- Perf, if performance analysis is needed.

On Ubuntu, the system dependencies can be installed with:

```bash
sudo apt update
sudo apt install postgresql postgresql-client libpq-dev valgrind linux-tools-common linux-tools-generic
```

## Database

Create a PostgreSQL database and user for the application. Example:

```sql
CREATE USER calculator_app WITH PASSWORD 'your_password';
CREATE DATABASE calculator_db OWNER calculator_app;
GRANT ALL PRIVILEGES ON DATABASE calculator_db TO calculator_app;
```

If schema permissions are needed:

```sql
GRANT ALL ON SCHEMA public TO calculator_app;
```

The application creates the `calculations` table automatically:

```sql
CREATE TABLE IF NOT EXISTS calculations (
    id SERIAL PRIMARY KEY,
    operation TEXT NOT NULL,
    value1 INTEGER NOT NULL,
    value2 INTEGER,
    result INTEGER,
    status INTEGER NOT NULL
);
```

`status = 0` means success. Non-zero statuses represent calculation errors.

## Configuration

The application reads database settings from `config.json` in the current
working directory.

Create a local config from the example:

```bash
cp config.example.json config.json
```

Then edit `config.json`:

```json
{
  "database": {
    "host": "localhost",
    "port": 5432,
    "dbname": "calculator_db",
    "user": "calculator_app",
    "password": "your_password"
  }
}
```

`config.json` is ignored by git because it contains local credentials.

## Build

```bash
cmake -B build
cmake --build build
```

After build, the executable is available at:

```text
build/calculator
```

## Tests

The project includes tests for successful calculations, calculation errors,
invalid JSON input, and help output. CTest also runs the test executable under
Valgrind when Valgrind is installed.

Run tests with:

```bash
ctest --test-dir build --output-on-failure
```

The tests expect `config.json` to exist in the project root.

## Install (optional)

To install the binary system-wide:

```bash
sudo cmake --build build --target install
```

This installs the executable to:

```text
/usr/local/bin/calculator
```

The installed executable still reads `config.json` from the current working
directory.

## Usage

```text
calculator '<json>'
calculator -h
calculator --help
```

When running from the build directory:

```bash
./build/calculator '{"operation":"add","value1":1,"value2":2}'
```

## JSON format

Binary operations use `operation`, `value1`, and `value2`:

```json
{"operation":"add","value1":1,"value2":2}
```

Factorial uses `operation` and `value1`:

```json
{"operation":"factorial","value1":5}
```

Supported operations:

```text
add
subtract
multiply
divide
power
factorial
```

All values must be JSON integer numbers. Malformed JSON, missing required
fields, unsupported operations, and invalid value types are reported as errors.

## Examples

```text
calculator '{"operation":"add","value1":1,"value2":2}' -> 1 + 2 = 3
calculator '{"operation":"subtract","value1":-5,"value2":-2}' -> (-5) - (-2) = -3
calculator '{"operation":"multiply","value1":5,"value2":-6}' -> 5 * (-6) = -30
calculator '{"operation":"divide","value1":10,"value2":2}' -> 10 / 2 = 5
calculator '{"operation":"power","value1":2,"value2":3}' -> 2 ^ 3 = 8
calculator '{"operation":"factorial","value1":3}' -> 3! = 6
```

Negative operands are printed in parentheses to keep expressions readable:

```text
5 * (-6) = -30
(-5) - (-2) = -3
```

## Inspecting Stored Data

Use `psql` to inspect calculation history:

```bash
psql -h localhost -U calculator_app -d calculator_db
```

Inside `psql`:

```sql
SELECT * FROM calculations;
```

To clear the table for a demo:

```sql
TRUNCATE TABLE calculations RESTART IDENTITY;
```

## Valgrind

Manual memory check example:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./build/calculator '{"operation":"add","value1":1,"value2":2}'
```

The CTest suite also includes a Valgrind check when `valgrind` is available:

```bash
ctest --test-dir build --output-on-failure
```

## Perf

Basic performance statistics:

```bash
sudo perf stat -e task-clock,context-switches,cpu-migrations,page-faults ./build/calculator '{"operation":"multiply","value1":123,"value2":456}'
```

A repeated operation can be measured with the same command to compare a cache
miss and a cache hit. In this CLI application, most runtime is expected to be
spent on process startup, database connection, table initialization, and cache
warming rather than on arithmetic itself.

## Logging

Calculation requests, results, and errors are written to:

```text
calculator.log
```

The file is created in the working directory from which the program is run.

## Errors

Possible error messages:

```text
Error! Expected one JSON argument
Error! Missing required JSON fields: operation or value1
Error! Missing required JSON fields: value2
Error! Division by zero
Error! Unknown operation
Error! Raising to a negative power
Error! Factorial of a negative number
Error! Type overflow
```

Invalid JSON syntax and values of the wrong JSON type are also reported as
errors with details supplied by the JSON parser.
