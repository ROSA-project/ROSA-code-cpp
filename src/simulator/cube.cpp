#include "cube.hpp"
#include "box.hpp"

namespace rosa {

Cube::Cube()
    : Cube(0, 0, 0) {}

Cube::Cube(float length, float height, float width)
    : length_(length)
    , height_(height)
    , width_(width) {}

std::shared_ptr<Box> Cube::boundingBox(const Position& position) { return nullptr; }

} // namespace rosa