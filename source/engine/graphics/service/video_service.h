#pragma once

#include <core/service.h>
#include <core/result.h>

namespace kairos {

class video_service : public service {
  public:
    result<uint32> enable();
    result<uint32> cleanup();
};

} // namespace kairos