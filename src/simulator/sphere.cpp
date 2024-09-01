#include "sphere.hpp"

namespace rosa {

Sphere::Sphere()
    : Sphere(0) {}

Sphere::Sphere(float radius)
    : radius_(radius) {}

std::shared_ptr<Box> Sphere::boundingBox(const Position& position) { return nullptr; }

} // namespace rosa