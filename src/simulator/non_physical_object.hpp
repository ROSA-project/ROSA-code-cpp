#pragma once

#include "object.hpp"

namespace rosa {

/**
 * Object without physical existence, its intersection with other objects does not cause
 * bumps.
 */
class NonPhysicalObject : public Object {};

} // namespace rosa