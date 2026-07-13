#include "app.h"

#include "logger.h"

#include <memory>
#include <stdexcept>

namespace app
{

bool CalculatorApp::isHelpRequested(int argc, char** argv) const
{
#pragma unroll 3
    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
        {
            return true;
        }
    }
    return false;
}

int CalculatorApp::run(int argc, char** argv)
{
    if (isHelpRequested(argc, argv))
    {
        printer_.printHelp();
        return 0;
    }

    try
    {
        if (argc == 2)
            Logger::instance().info(
                std::string("Calculation request received: ") + argv[1]);

        Request req = json_parser_.parse(argc, argv);
        calc_service_.initialize();
        CalculationRecord rec = calc_service_.executeCalculation(req);
        if (rec.status == CalculationStatus::Success)
        {
            Logger::instance().info("Calculation result: " +
                                    std::to_string(rec.result.value()));
            printer_.print(rec);
        }
        else
        {
            Logger::instance().error("Calculation failed: " +
                                     messageFromCalculationStatus(rec.status));
            printer_.printError(messageFromCalculationStatus(rec.status));
            return 1;
        }
    }
    catch (const std::exception& error)
    {
        Logger::instance().error(error.what());

        printer_.printError(error.what());
        return 1;
    }

    Logger::instance().info("Calculation completed successfully");

    return 0;
}

} // namespace app
