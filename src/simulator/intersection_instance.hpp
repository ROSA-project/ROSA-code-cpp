#pragma once

#include "common/util.hpp"
#include "object.hpp"

namespace rosa {

/**
 * Performs intersection between two objects and stores the result.
 * TODO: Currently a sketch. Interface no defined right.
 */
class IntersectionInstance {
public:
    IntersectionInstance(const Object& obj1, const Object& obj2);

    /**
     * The entry point for performing the intersection algorithm on its two objects.
     */
    void intersect();

    /**
     * Merely provides access to internal attribute
     * TODO: fix the comment
     */
    bool doesIntersect() { return doesIntersect_; }

    bool isInfinitesimal() {
        rosa_assert(doesIntersect_,
                    "isInfinitesimal should be called only if doesIntersect is set!");
        return isInfinitesimal_;
    }

    const std::vector<Position>& getIntersectionPoint() {
        rosa_assert(intersectionPoints_.size() > 0, "no intersection recorded.");
        return intersectionPoints_;
    }

private:
    const Object& obj1_;
    const Object& obj2_;
    bool doesIntersect_{false};
    bool isInfinitesimal_{false};
    std::vector<Position> intersectionPoints_; // TODO: not sure about the type of this
};

} // namespace rosa