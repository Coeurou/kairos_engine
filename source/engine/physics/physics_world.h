#pragma once

#include <core/types.h>

namespace kairos {

struct dynamic {
    vec2f my_position = {0.f, 0.f};
    vec2f my_velocity = {0.f, 0.f};
};

class physics_world {
  public:
    array<dynamic>& dynamics();
    const array<dynamic>& dynamics() const;

    void apply_gravity(float dt);

    void integrate(float dt);

  private:
    float my_gravity = -9.81f;
    array<dynamic> my_dynamic_objects;
};

} // namespace kairos