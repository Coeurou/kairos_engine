#pragma once

#include <physics/types.h>

namespace kairos {

/** A particle is the simplest object simulated in the physics module */
class particle {
  public:
    /** Member functions */
    particle() = default;
    particle(real_vec2 pos, real_vec2 vel = real_vec2{0.f, 0.f}, real mass = 1.f, real damping = 0.99f);

    const real_vec2& position() const;
    void set_position(real_vec2 pos);

    const real_vec2& velocity() const;
    void set_velocity(real_vec2 vel);

    const real_vec2& applied_forces() const;
    void apply_force(real_vec2 force);
    void clear_applied_force();

    real inverse_mass() const;
    void set_inverse_mass(real inv_mass);

    real mass() const;
    void set_mass(real mass);

    real damping() const;
    void set_damping(real damping);

  private:
    /** particle position in world space */
    real_vec2 my_position{0.f};
    /** particle velocity in m/s */
    real_vec2 my_velocity{0.f};
    /** sum of forces applied to a particle each time integrate is called */
    real_vec2 my_applied_forces{0.f};
    /** particle inverse mass, simplify acceleration computation and describe more accurately an
     * object with an infinite mass (inverse mass is 0) */
    real my_inverse_mass{0.f};
    /** damping applied to linear motion. It is used to mitigate the precision error of the
     * integration */
    real my_damping{1.f};
};

/** Non-member functions */

/** apply any form of gravity, convert parameter gravity to a real_vec2 where Y component is gravity
 * value */
void apply_gravity(particle& p, real gravity);

/** perform a particle integration. Compute acceleration (inverse_mass * applied_forces),
 * apply damping to velocity (velocity *= damping), integrate velocity (velocity += acceleration *
 * dt), integrate position (position += velocity * dt), finally, clear applied forces. dt must be
 * stable in order to keep integration operation valid */
void integrate(particle& p, time_span dt);

constexpr bool operator==(const particle& lhs, const particle& rhs) {
    return lhs.position() == rhs.position() && lhs.velocity() == rhs.velocity() &&
        lhs.applied_forces() == rhs.applied_forces() &&
        lhs.inverse_mass() == rhs.inverse_mass() && lhs.damping() == rhs.damping();
}

constexpr bool operator!=(const particle& lhs, const particle& rhs) {
    return !(lhs == rhs);
}

} // namespace kairos