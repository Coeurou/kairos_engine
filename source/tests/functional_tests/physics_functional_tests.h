#pragma once

#include <core/types.h>
#include <physics/particle.h>
#include <physics/types.h>

std::pair<uint32, uint32> test_ballistic();

namespace kairos {

class firework {
  public:
    enum class type { parent, child };
    /** physics properties of the firework */
    particle my_rocket;
    /** the age of a firework determines when it detonates. Age gradually
     * decreases; when it passes zero the firework delivers its payload.
     */
    time_span my_age = 0.f;
    /** a parent firework will spawn several children on detonation */
    type my_type = type::parent;
};

constexpr bool operator==(const firework& lhs, const firework& rhs) {
    return lhs.my_rocket == rhs.my_rocket && lhs.my_type == rhs.my_type;
}

constexpr bool operator!=(const firework& lhs, const firework& rhs) {
    return !(lhs == rhs);
}

void update(firework& f, time_span dt);

struct explosion_params {
    std::pair<uint32, uint32> my_children_count = {3, 9};
    std::pair<time_span, time_span> my_age_range = {1.f, 3.f};
    std::pair<real, real> my_horizontal_velocity_range = {-45.f, 45.f};
};

void explode(firework& f, array<firework>& fireworks, const explosion_params& params);
} // namespace kairos

std::pair<uint32, uint32> test_firework();