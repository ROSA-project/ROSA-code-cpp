#pragma once

#include "object.hpp"
#include "utils/util.hpp"

namespace rosa {
// TODO move
double const epsilon_criterion = 0.1;
double const non_zero_criterion = 0.0001;

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
    bool doesIntersect() {
        return doesIntersect_;
    }

    bool isInfinitesimal() {
        rosa_assert(doesIntersect_, "isInfinitesimal should be called only if doesIntersect is set!");
        return isInfinitesimal_;
    }

private:
    const Object& obj1_;
    const Object& obj2_;
    bool doesIntersect_ {false};
    bool isInfinitesimal_ {false};
};

} // namespace rosa