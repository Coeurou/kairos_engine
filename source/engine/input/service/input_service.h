#pragma once

#include <core/event.h>
#include <core/result.h>
#include <core/service.h>
#include <input/controller.h>
#include <input/keyboard.h>

namespace kairos {

enum class input_device {
    controller,
};

/** input_service is responsible of managing lifetime and status of input devices.
 * It handles a keyboard and one controller.
 */
class input_service : public service {
  public:
    /** Member functions */

    result<uint32> enable(input_device device);
    void process_events(const array<system_event>& events);

  private:
    keyboard my_keyboard;
    controller my_controller;
};

} // namespace kairos