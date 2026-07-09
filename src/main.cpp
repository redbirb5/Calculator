#include "config.h"

#include "app.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    try
    {
        const std::string connection_string =
            app::readConnectionString("config.json");
        app::CalculatorApp calc(connection_string);
        return calc.run(argc, argv);
    }
    catch (const std::exception& err)
    {
        std::cerr << "Error! " << err.what() << "\n";
        return 1;
    }
}
