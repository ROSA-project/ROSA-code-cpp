#pragma once

#include "common/util.hpp"
#include "object.hpp"
#include <vector>
#include <utility>
#include "position.hpp"

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
    double intersect();
    std::pair<std::vector<double>, std::vector<double>> reversion();
    /**
     * Merely provides access to internal attribute
     * TODO: fix the comment
     */
    bool doesIntersect(){return doesIntersect_;};

    bool isInfinitesimal() {
        rosa_assert(doesIntersect_,
                    "isInfinitesimal should be called only if doesIntersect is set!");
        return isInfinitesimal_;
    }

    Position IntersectionPoint(const Object& obj1, const Object& obj2) {}
    const Position getIntersectionPoint() {
        // rosa_assert(intersectionPoints_.size() > 0, "no intersection recorded.");
        return intersectionPoint_;
    }

private:
    const Object& obj1_;
    const Object& obj2_;
    bool doesIntersect_{false};
    bool isInfinitesimal_{false};
    Position intersectionPoint_; // TODO: not sure about the type of this
};

} // namespace rosa