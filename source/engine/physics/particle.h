#pragma once

#include <physics/types.h>

namespace kairos {

/** A particle is the simplest object simulated in the physics module */
class particle {
  public:
    /** Member functions */
    particle() = default;
    particle(real_vec2 pos, real_vec2 vel = real_vec2{0.f, 0.f}, real mass = 1.f,
             real damping = 0.99f);

    const real_vec2& position() const;
    /** modify current position. mainly use for particle integration when velocity is applied to
     * position. */
    void set_position(real_vec2 pos);

    /** return particle velocity in m/s in all directions. velocity is used for particle position
     * integration */
    const real_vec2& velocity() const;
    /** modify current velocity. mainly use for particle integration when acceleration is applied to
     * velocity. */
    void set_velocity(real_vec2 vel);

    /** return the sum of the forces currently applied to this particle */
    const real_vec2& applied_forces() const;
    /** add a force to be considered when particle integration occurs. remember that forces are
     * cleared each time integrate is called */
    void apply_force(real_vec2 force);
    /** clear all the forces applied to a particle. called automatically by integrate function. */
    void clear_applied_force();

    /** return particle inverse mass. inverse mass provide an easier way to compute particle
     * acceleration during integration. */
    real inverse_mass() const;
    void set_inverse_mass(real inv_mass);

    /** return particle mass as the inverse of inverse mass member. mass is used when computing
     * particle acceleration. */
    real mass() const;
    void set_mass(real mass);

    /** return particle damping, which is applied to velocity during integration */
    real damping() const;
    /** modify current velocity. mainly use for particle integration when velocity is updated
     * (velocity *= pow(damping, dt)). */
    void set_damping(real damping);

  private:
    /** particle position in world space */
    real_vec2 my_position{0.f};
    /** particle velocity in m/s */
    real_vec2 my_velocity{0.f};
    /** sum of forces applied to a particle each time integrate is called. this value will be
     * cleared at the end of integrate function*/
    real_vec2 my_applied_forces{0.f};
    /** particle inverse mass, simplify acceleration computation and describe more accurately an
     * object with an infinite mass (inverse mass is 0) */
    real my_inverse_mass{0.f};
    /** damping applied to linear motion. it is used to mitigate the precision error of the
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

constexpr bool operator!=(const particle& lhs, const particle& rhs) { return !(lhs == rhs); }

} // namespace kairos