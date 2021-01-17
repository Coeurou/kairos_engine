#include <physics/particle_force_registry.h>

namespace kairos {

void update(particle_force_registry& registry, time_span dt) {
    for (auto& entry : registry) {
        apply_force(entry.my_force, *entry.my_particle);
    }
}

}