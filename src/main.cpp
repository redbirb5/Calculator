#include "app.h"

#include <string>

int main(int argc, char** argv)
{
    const std::string connection_string =
        "host=localhost port=5432 dbname=calculator_db user=calculator_app password=1212";
    app::CalculatorApp calc(connection_string);
    return calc.run(argc, argv);
}
