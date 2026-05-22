# calculator

Simple CLI calculator in C++ using the `libmath` library.

## Build

```bash
cmake -B build
cmake --build build
```

After build, the executable is available at:

```text
build/calculator
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

## Errors

Possible error messages:

```text
Error! Division by zero
Error! Unknown operation
Error! Raising to a negative power
Error! Factorial of a negative number
Error! Type overflow
```
