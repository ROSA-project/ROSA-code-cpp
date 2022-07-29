#include "cylinder.hpp"

namespace rosa {

Cylinder::Cylinder()
    : Cylinder(0, 0) {}

Cylinder::Cylinder(float radius, float height)
    : radius_(radius)
    , height_(height) {}

std::shared_ptr<Box> Cylinder::boundingBox(const Position& position) { return nullptr; }

} // namespace rosa