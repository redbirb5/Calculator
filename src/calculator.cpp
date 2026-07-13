#include "calculator.h"

#include "libmath.h"

#include <stdexcept>

namespace app
{

class Calculator::Impl
{
  public:
    libmath::Arithmetic math_;
};

Calculator::Calculator() : impl_(std::make_unique<Impl>())
{}

Calculator::~Calculator() = default;

int Calculator::calculate(const Request& req) const
{
    switch (req.operation)
    {
        case Operation::Add:
            return impl_->math_.add(req.value1, req.value2.value());
        case Operation::Subtract:
            return impl_->math_.subtract(req.value1, req.value2.value());
        case Operation::Multiply:
            return impl_->math_.multiply(req.value1, req.value2.value());
        case Operation::Divide:
            return impl_->math_.divide(req.value1, req.value2.value());
        case Operation::Power:
            return impl_->math_.power(req.value1, req.value2.value());
        case Operation::Factorial:
            return impl_->math_.factorial(req.value1);
        default:
            throw std::invalid_argument("Unknown operation");
    }
}

} // namespace app