#include <iostream>

int addition(int a, int b)
{
    return a + b;
}

int substraction(int a, int b)
{
    return a - b;
}

int multiplication(int a, int b)
{
    return a * b;
}

int division(int a, int b, int& c)
{
    if(b == 0)
    {
	return -1;
    }
    c = a / b;
    return 0;
}

void printPesult(int a, int b, int c, char operation)
{
     std::cout << a << operation << b << " = " << c << '\n';
}

int main()
{
    int a = 1;
    int b = 2;
    int c = 0;

    c = addition(a, b);
    printResult(a, b ,c, '+');
    
    c = substraction(a, b);    
    printResult(a, b ,c, '-');
    
    c = multiplication(a, b);
    printResult(a, b ,c, '*');
	
    int status = 0;
    status = division(a, b, c);
    if(status)
    {
	printResult(a, b, c, '/');
    }
    else
    {
	 std::cout << "Error! Division by zero is not allowed";
    }

    c = 1;
    for (int i = 0; i < b; ++i)
    {
	c = c * a;
    }
    std::cout << a << " ^ " << b << " = " << c << '\n';

}
