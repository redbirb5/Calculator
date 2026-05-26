# calculator

Simple CLI calculator in C++ using the `libmath` library.

Current version: `2.0.0`.

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

The project includes unit tests for successful calculations, calculation errors,
invalid JSON input, and help output.

After building the project, run the tests with:

```bash
ctest --test-dir build --output-on-failure
```

## Install (optional)

To install the binary system-wide:

```bash
sudo cmake --build build --target install
```

This will install the executable to:

```text
/usr/local/bin/calculator
```

## Usage

```text
calculator '<json>'
calculator -h
calculator --help
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

All values must be JSON integer numbers. Malformed JSON, missing required fields,
unsupported operations, and invalid value types are reported as errors.

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

Invalid JSON syntax and values of the wrong JSON type are also reported as errors
with details supplied by the JSON parser.
