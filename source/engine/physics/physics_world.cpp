#include <physics/physics_world.h>

namespace kairos {

array<dynamic>& physics_world::dynamics() { return my_dynamic_objects; }

const array<dynamic>& physics_world::dynamics() const { return my_dynamic_objects; }

void physics_world::apply_gravity(float dt) {
    for (auto& d : my_dynamic_objects) {
        d.my_velocity.y += my_gravity * dt;
    }
}

void physics_world::integrate(float dt) {
    for (auto& d : my_dynamic_objects) {
        d.my_position += d.my_velocity * dt;
    }
}

} // namespace kairos