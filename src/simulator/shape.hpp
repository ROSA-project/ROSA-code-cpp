#pragma once

#include "nlohmann/json.hpp"
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

    virtual bool isShapeless() const = 0;

    virtual nlohmann::json toJson() const = 0;

    virtual void fromJson(const nlohmann::json& j) = 0;
};

} // namespace rosa