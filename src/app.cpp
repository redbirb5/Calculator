#include "app.h"

#include "libmath.h"

#include <getopt.h>
#include <string.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>

namespace
{

struct Task
{
    int value1;
    char operation;
    int value2;
    int status;
    int result;
};

void parse(int argc, char** argv, Task& task)
{
    const option long_options[] = {{"add", no_argument, nullptr, 'a'},
                                   {"subtract", no_argument, nullptr, 's'},
                                   {"multiply", no_argument, nullptr, 'm'},
                                   {"divide", no_argument, nullptr, 'd'},
                                   {"power", no_argument, nullptr, 'p'},
                                   {"factorial", no_argument, nullptr, 'f'},
                                   {nullptr, 0, nullptr, 0}};

    int opt;
    int option_index = 0;

    bool operation_found = false;

    // NOLINTNEXTLINE(altera-unroll-loops)
    while ((opt = getopt_long(argc, argv, "asmdpf", long_options,
                              &option_index)) != -1)
    {
        switch (opt)
        {
            case 'a':
            case 's':
            case 'm':
            case 'd':
            case 'p':
            case 'f':
                if (operation_found)
                {
                    task.operation = '?';
                    return;
                }

                operation_found = true;
                switch (opt)
                {
                    case 'a':
                        task.operation = '+';
                        break;
                    case 's':
                        task.operation = '-';
                        break;
                    case 'm':
                        task.operation = '*';
                        break;
                    case 'd':
                        task.operation = '/';
                        break;
                    case 'p':
                        task.operation = '^';
                        break;
                    case 'f':
                        task.operation = '!';
                        break;
                }

                break;

            default:
                task.operation = '?';
                return;
        }
    }

    if (task.operation == '!')
    {
        task.value1 = atoi(argv[optind]);
    }
    else
    {
        task.value1 = atoi(argv[optind]);
        task.value2 = atoi(argv[optind + 1]);
    }
}

void calculate(Task& task)
{
    task.status = 0;
    switch (task.operation)
    {
        case '+':
            task.status =
                libmath::addition(task.value1, task.value2, task.result);
            break;
        case '-':
            task.status =
                libmath::subtraction(task.value1, task.value2, task.result);
            break;
        case '*':
            task.status =
                libmath::multiplication(task.value1, task.value2, task.result);
            break;
        case '/':
            task.status =
                libmath::division(task.value1, task.value2, task.result);
            break;
        case '^':
            task.status = libmath::power(task.value1, task.value2, task.result);
            break;
        case '!':
            task.status = libmath::factorial(task.value1, task.result);
            break;
        default:
            task.status = 1;
    }
}

void output(Task task)
{
    if (task.status == 0)
    {
        if (task.operation == '!')
        {
            printf("%d! = %d\n", task.value1, task.result);
        }
        else
        {
            printf("%d %c %d = %d\n", task.value1, task.operation, task.value2,
                   task.result);
        }
    }
    else if (task.status == -1)
    {
        printf("Error! Division by zero!\n");
    }
    else if (task.status == 1)
    {
        printf("Error! Unknown operation!\n");
    }
    else if (task.status == 2)
    {
        printf("Error! Raising to a negative power!\n");
    }
    else if (task.status == 3)
    {
        printf("Error! Factorial of a negative number!\n");
    }
    else if (task.status == 4)
    {
        printf("Error! Type overflow!\n");
    }
    else
    {
        printf("Unknown error\n");
    }
}

bool isHelpRequested(int argc, char** argv)
{
#pragma unroll 3
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            return true;
        }
    }
    return false;
}

void printHelp()
{
    const char help[] =
        "Usage:\n"
        "  calculator [option] <value1> <value2>\n"
        "  calculator -f <value>\n"
        "  calculator --factorial <value>\n\n"
        "Options:\n"
        "  -a, --add          Add\n"
        "  -s, --subtract     Subtract\n"
        "  -m, --multiply     Multiply\n"
        "  -d, --divide       Divide\n"
        "  -p, --power        Power\n"
        "  -f, --factorial    Factorial (one argument)\n"
        "  -h, --help         Show help\n\n"
        "Notes:\n"
        "  Use \"--\" once to stop option parsing if arguments are negative numbers.\n"
        "  All arguments after \"--\" are treated as values.\n"
        "  Example: calculator -s -- -5 -2\n"
        "  Example: calculator --subtract -- -5 -2\n\n"
        "Examples:\n"
        "  calculator -a 1 2              -> 1 + 2 = 3\n"
        "  calculator --add 1 2           -> 1 + 2 = 3\n"
        "  calculator -s -- -5 -2         -> -5 - -2 = -3\n"
        "  calculator --subtract -- -5 -2 -> -5 - -2 = -3\n"
        "  calculator -m 3 4              -> 3 * 4 = 12\n"
        "  calculator --multiply 3 4      -> 3 * 4 = 12\n"
        "  calculator -d 10 2             -> 10 / 2 = 5\n"
        "  calculator --divide 10 2       -> 10 / 2 = 5\n"
        "  calculator -p 2 3              -> 2 ^ 3 = 8\n"
        "  calculator --power 2 3         -> 2 ^ 3 = 8\n"
        "  calculator -f 3                -> 3! = 6\n"
        "  calculator --factorial 3       -> 3! = 6\n";

    write(STDOUT_FILENO, help, sizeof(help) - 1);
}

} // namespace

namespace app
{

void run(int argc, char** argv)
{
    if (isHelpRequested(argc, argv))
    {
        printHelp();
        return;
    }

    Task task;
    parse(argc, argv, task);
    calculate(task);
    output(task);
}

} // namespace app
