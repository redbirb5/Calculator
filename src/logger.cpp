#include "logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

namespace app
{

class Logger::Impl
{
  public:
    std::shared_ptr<spdlog::logger> logger;
};

Logger::Logger() : impl_(std::make_unique<Impl>())
{
    impl_->logger =
        spdlog::basic_logger_mt("calculator_logger", "calculator.log");
}

Logger::~Logger() = default;

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::info(const std::string& message)
{
    impl_->logger->info(message);
}

void Logger::error(const std::string& message)
{
    impl_->logger->error(message);
}

} // namespace app