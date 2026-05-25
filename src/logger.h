#pragma once

#include <memory>
#include <string>

namespace app
{

class Logger
{
  public:
    static Logger& instance();

    void info(const std::string& message);
    void error(const std::string& message);

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

} // namespace app