#pragma once

#include "position.hpp"
#include <memory>

namespace rosa {

class Box;

class Shape {
public:
    virtual ~Shape() = default;

    /**
     * Returns smallest enclosing upright Box
     *
     * @param position Current position of the shape
     * @return A shred pointer to the resulting enclosing box
     */
    virtual std::shared_ptr<Box> boundingBox(const Position& position) = 0;

    /**
     * Returns the shape info needed for visualization
     *
     * @return A dictionary with two keys: "type" is the shape's name, and "dimension" is
     * a list of dimension numbers.
     */
    virtual void dumpInfo() const;

    virtual bool isShapeless() const { return true; }
};

} // namespace rosa