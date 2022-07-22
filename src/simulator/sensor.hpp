#pragma once

#include "object.hpp"

namespace rosa {

class Sensor : public Object {
public:
    virtual bool sense() = 0;
};

} // namespace rosa