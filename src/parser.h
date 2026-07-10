#pragma once

#include "calculation_types.h"

namespace app
{
class Parser
{
  public:
    Request parse(int argc, char** argv) const;
};
} // namespace app