#pragma once

#include "object.hpp"

namespace rosa {

class Sensor : public Object {
public:
    // Inherit Object's constructor
    using Object::Object;
    
    virtual bool sense() = 0;

    // TODO: bounding box of sensor?
    virtual std::shared_ptr<Box> boundingBox() {
        return nullptr;
    }

};

} // namespace rosa