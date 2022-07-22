#include "intersection_instance.hpp"

namespace rosa {

IntersectionInstance::IntersectionInstance(const Object& obj1, const Object& obj2)
: obj1_(obj1)
, obj2_(obj2) {
    
    intersect();
}

void IntersectionInstance::intersect() {
    // TODO a temporary implementation for pairs of supported geometrical shapes
}


} // namespace rosa