# calculator

Simple CLI calculator in C++ using the `libmath` library (git repository: git@github.com:guelderrose8/libmath.git).

## Build

cmake -B build
cmake --build build

After build, the executable is available at:

build/calculator


## Install (optional)

To install the binary system-wide:

sudo cmake --build build --target install

This will install the executable to:

/usr/local/bin/calculator


## Usage

calculator [options] <value1> [value2]


## Options

-a    Add  
-s    Subtract  
-m    Multiply  
-d    Divide  
-p    Power  
-f    Factorial (one argument)  
-h    Show help


## Notes

Use `--` once to stop option parsing if arguments are negative numbers.
All arguments after `--` are treated as values.

Example:

calculator -s -- 5 -2


## Examples

calculator -a 1 2        -> 1 + 2 = 3  
calculator -s -- -5 -2   -> -5 - -2 = -3  
calculator -m 3 4        -> 3 * 4 = 12  
calculator -d 10 2       -> 10 / 2 = 5  
calculator -p 2 3        -> 2 ^ 3 = 8  
calculator -f 3          -> 3! = 6  


## Errors

Possible error messages:

Error! Division by zero!  
Error! Unknown operation!  
Error! Raising to a negative power!  
Error! Factorial of a negative number!  
Error! Type overflow!  
Unknown error
