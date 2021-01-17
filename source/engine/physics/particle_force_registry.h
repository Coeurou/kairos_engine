#pragma once

#include <physics/force.h>
#include <physics/types.h>

namespace kairos {

class particle;

struct particle_force_entry {
    particle* my_particle{ nullptr };
    force my_force{};
};

using particle_force_registry = array<particle_force_entry>;

void update(particle_force_registry& registry, time_span dt);

} // namespace kairos