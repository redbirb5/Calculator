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
        app::CalculatorApp calc_app(connection_string);
        return calc_app.run(argc, argv);
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error! " << error.what() << "\n";
        return 1;
    }
}
