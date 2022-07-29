#pragma once

#include "object.hpp"

namespace rosa {

class Robot : public Object {
public:
    // Inherits the Object's constructor
    using Object::Object;
};

} // namespace rosa