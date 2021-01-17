#pragma once

#include <physics/types.h>

namespace kairos {

class particle;

/** simulate a drag force, one instance can apply drag to multiple particles */
class drag_force {
  public:
    /** Member functions */

    drag_force() = default;
    constexpr drag_force(real k1, real k2) : my_coefficients({k1, k2}) {}

    /** return drag coefficients k1 & k2 as a pair of real numbers. drag coefficients are used for
     * computing drag force (see apply_force) */
    constexpr const std::pair<real, real>& drag_coefficients() const { return my_coefficients; }

  private:
    /** drag coefficients k1 & k2, help generate drag coeffient to be applied on particle velocity
     * (k1 * speed + k2 * speed* speed) */
    std::pair<real, real> my_coefficients = {0.f, 0.f};
};

/** Non-member functions */

/** compute drag force based on drag coefficients (k1 * speed + k2 * speed* speed). drag force is in
 * the opposite direction of particle velocity. call particle::apply_force to enable result for
 * integration */
void apply_force(drag_force& drag, particle& p);

} // namespace kairos