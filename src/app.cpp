#include "app.h"
#include "libmath.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

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
    int opt;

    while ((opt = getopt(argc, argv, "asmdpf") != -1) {
switch (opt) {
case 'a': task.operation = '+'; break;
case 's': task.operation = '-'; break;
            case 'm': task.operation = '*'; break;
            case 'd': task.operation = '/'; break;
            case 'p': task.operation = '^'; break;
            case 'f': task.operation = '!'; break;
            default: task.operation = '?'; return;
}
}
	
    if(task.operation == '!'){
    task.value1 = atoi(argv[optind]);
    }
    else {
    task.value1 = atoi(argv[optind]);
    task.value2 = atoi(argv[optint + 1]);
    }    
}

void calculate(Task& task)
{
    task.status = 0;
    switch(task.operation)
    {
        case '+':
                task.status = libmath::addition(task.value1, task.value2, task.result);
                break;
        case '-':
                task.status = libmath::subtraction(task.value1, task.value2, task.result);
                break;
        case '*':
                task.status = libmath::multiplication(task.value1, task.value2, task.result);
                break;
        case '/':
                task.status = libmath::division(task.value1, task.value2, task.result);
                break;
        case '^':
                task.status = libmath::power(task.value1, task.value2, task.result);
                break;
        default:
                task.status = 1;

    }
}

void output(Task task)
{
    if(task.status == 0)
    {
	if(task.operation == '!')
	{
		printf("%d! = %d\n", task.value1, task.result);
	}else {
             printf("%d %c %d = %d\n", task.value1, task.operation, task.value2, task.result);
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

}

namespace app
{

void run(int argc, char** argv)
{
    Task task;
    parse(argc, argv, task);
    calculate(task);
    output(task);
}

}
