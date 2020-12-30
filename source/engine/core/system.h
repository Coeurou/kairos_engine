#pragma once

#include <core/result.h>
#include <core/types.h>

class system {
  public:
    virtual result<uint32> setup() = 0;
    virtual result<uint32> update() = 0;
    virtual result<uint32> cleanup() = 0;
};