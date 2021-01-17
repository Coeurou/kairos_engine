#include <physics/drag_force.h>

#include <math/mathlib.h>
#include <physics/particle.h>

namespace kairos {

/** Non-member functions */

void apply_force(drag_force& drag, particle& p) {
    const auto& coefficients = drag.drag_coefficients();
    const real magnitude = length(p.velocity());
    const real d =
        coefficients.first * magnitude + coefficients.second * magnitude * magnitude;

    p.apply_force(p.velocity() / magnitude * -d);
}

} // namespace kairos