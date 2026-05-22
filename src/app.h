#pragma once

#include <memory>
#include <optional>
#include <string>

namespace app
{
enum class Operation
{
    Unknown = 0,
    Add,
    Subtract,
    Multiply,
    Divide,
    Power,
    Factorial,
};

struct Request
{
    Operation operation;
    int value1;
    std::optional<int> value2;
};

class JsonParser
{
  public:
    Request parse(int argc, char** argv) const;

  private:
    Operation recognizeOperation(const std::string& operation) const;
};

class Calculator
{
  public:
    Calculator();
    ~Calculator();

    int calculate(const Request& request) const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

class Printer
{
  public:
    void print(const Request& request, int result) const;
    void printHelp() const;
    void printError(const std::string& error_message) const;

  private:
    std::string getOperationSymbol(Operation oprt) const;
    std::string formatOperand(int operand) const;
};

class CalculatorApp
{
  public:
    int run(int argc, char** argv);

  private:
    bool isHelpRequested(int argc, char** argv) const;
    JsonParser json_parser_;
    Calculator calculator_;
    Printer printer_;
};

} // namespace app
