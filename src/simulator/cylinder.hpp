#pragma once

#include "shape.hpp"
#include <memory>

namespace rosa {

class Box;

class Cylinder : public Shape {
public:
    Cylinder(float radius, float height);

    std::shared_ptr<Box> boundingBox(const Position& position) override;

    float getRadius() const { return radius_; }

    float getHeight() const { return height_; }

private:
    float radius_;
    float height_;
};

} // namespace rosa