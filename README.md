# calculator

Simple CLI calculator in C++ using the `libmath` library (git repository: `git@github.com:guelderrose8/libmath.git`).

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
calculator [option] <value1> <value2>
calculator -f <value>
calculator --factorial <value>
```

## Options

```text
-a, --add          Add
-s, --subtract     Subtract
-m, --multiply     Multiply
-d, --divide       Divide
-p, --power        Power
-f, --factorial    Factorial (one argument)
-h, --help         Show help
```

## Notes

Use `--` once to stop option parsing if arguments are negative numbers.
All arguments after `--` are treated as values.

Examples:

```bash
calculator -s -- -5 -2
calculator --subtract -- -5 -2
```

Use one operation option per command.

Correct:

```bash
calculator -a 1 2
calculator --add 1 2
```

Incorrect:

```bash
calculator -add 1 2
```

`-add` is interpreted as several short options: `-a`, `-d`, `-d`.
Use `--add` for the long option.

## Examples

```text
calculator -a 1 2              -> 1 + 2 = 3
calculator --add 1 2           -> 1 + 2 = 3

calculator -s -- -5 -2         -> -5 - -2 = -3
calculator --subtract -- -5 -2 -> -5 - -2 = -3

calculator -m 3 4              -> 3 * 4 = 12
calculator --multiply 3 4      -> 3 * 4 = 12

calculator -d 10 2             -> 10 / 2 = 5
calculator --divide 10 2       -> 10 / 2 = 5

calculator -p 2 3              -> 2 ^ 3 = 8
calculator --power 2 3         -> 2 ^ 3 = 8

calculator -f 3                -> 3! = 6
calculator --factorial 3       -> 3! = 6
```

## Errors

Possible error messages:

```text
Error! Division by zero!
Error! Unknown operation!
Error! Raising to a negative power!
Error! Factorial of a negative number!
Error! Type overflow!
Unknown error
```
