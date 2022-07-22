#include "cube.hpp"
#include "objects/box.hpp"

namespace rosa {

Cube::Cube(float length, float height, float width)
: length_(length)
, height_(height)
, width_(width) {
}

std::shared_ptr<Box> Cube::boundingBox(const Position& position) {
    return nullptr;
}


} // namespace rosa