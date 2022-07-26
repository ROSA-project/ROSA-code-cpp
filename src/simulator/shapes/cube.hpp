#pragma once

#include "shape.hpp"
#include <memory>

namespace rosa {

class Box;

class Cube : public Shape {
public:
    Cube(float length, float height, float width);

    std::shared_ptr<Box> boundingBox(const Position& position) override;

    float getLength() const { return radius_; }

    float getHeight() const { return height_; }

    float getWidth() const { return width_; }

private:
    float length_;
    float height_;
    float width_;
};

} // namespace rosa