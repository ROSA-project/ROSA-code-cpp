#pragma once

#include "shape.hpp"
#include <memory>

namespace rosa {

class Box;

class Cube: public Shape {
public:
    Cube(float length, float height, float width);

    std::shared_ptr<Box> boundingBox(const Position& position) override;

private:
    float length_;
    float height_;
    float width_;
};

} // namespace rosa