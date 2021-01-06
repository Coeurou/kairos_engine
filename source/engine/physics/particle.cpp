#include <core/contract.h>
#include <physics/particle.h>
#include "particle.h"

namespace kairos {

/** Member functions */

particle::particle(real_vec2 pos, real_vec2 vel, real mass, real damping)
    : my_position(std::move(pos)), my_velocity(std::move(vel)), my_inverse_mass(1.f / mass),
      my_damping(damping) {}

const real_vec2& particle::position() const { return my_position; }
void particle::set_position(real_vec2 pos) { my_position = std::move(pos); }

const real_vec2& particle::velocity() const { return my_velocity; }
void particle::set_velocity(real_vec2 vel) { my_velocity = std::move(vel); }

const real_vec2& particle::applied_forces() const { return my_applied_forces; }
void particle::apply_force(real_vec2 force) { my_applied_forces += std::move(force); }

void particle::clear_applied_force() { my_applied_forces = real_vec2{0.f, 0.f}; }

real particle::inverse_mass() const { return my_inverse_mass; }
real particle::mass() const { return 1.f / inverse_mass(); }
void particle::set_inverse_mass(real inv_mass) { my_inverse_mass = inv_mass; }
void particle::set_mass(real mass) { set_inverse_mass(1.f / mass); }

real particle::damping() const { return my_damping; }

void particle::set_damping(real damping) { my_damping = damping; }

/** Non-member functions */

void apply_gravity(particle& p, real gravity) { p.apply_force(real_vec2{0.f, gravity}); }

void integrate(particle& p, time_span dt) {
    expects(dt > 0.f);

    p.set_velocity((p.velocity() * p.damping()) + (p.applied_forces() * p.inverse_mass() * dt));
    p.set_position(p.velocity() * dt);
    p.clear_applied_force();
}

} // namespace kairos