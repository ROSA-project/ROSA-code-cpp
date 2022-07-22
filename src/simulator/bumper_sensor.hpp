#pragma once

#include "sensor.hpp"

namespace rosa {

/**
 * Object without physical existence, its intersection with other objects does not cause bumps. 
 */
class BumperSensor: public Sensor {
    virtual bool sense() override {
        return infinitesimalIntersectionOccured_;
    }
};

} // namespace rosa