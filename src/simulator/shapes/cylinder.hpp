#pragma once

#include "shape.hpp"
#include <memory>

namespace rosa {

class Box;

class Cylinder: public Shape {
public:
    Cylinder(float radius, float height);

    std::shared_ptr<Box> boundingBox(const Position& position) override;

private:
    float radius_;
    float height_;
};

} // namespace rosa