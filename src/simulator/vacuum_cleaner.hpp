#pragma once

#include "bumper_sensor.hpp"
#include "cylinder.hpp"
#include "object.hpp"
#include "object_registry.hpp"
#include "position.hpp"
#include "robot.hpp"
#include "sensor.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace rosa {

// Note: This class inherits from enable_shared_from_this in order to create weak_ptrs from "this" pointer. This is needed when creating the sensor object.

class VacuumCleaner: public Robot, public std::enable_shared_from_this<VacuumCleaner> {
public:
    VacuumCleaner(const ObjectId& oid,
           const std::string& name,
           std::unique_ptr<Cylinder>&& shape,
           const Position& position,
           const std::weak_ptr<Object>& owner_object,
           const std::shared_ptr<ObjectRegistry>& registry);

    void initializeSensor();

    virtual ObjectMap evolve(float delta_t) override;

    virtual float getRequiredDeltaT() const override;

private:
    enum PositionState {
        FORWARD,
        REVERSE,
        TURN_LEFT
    };

    bool isInitialized_;
    std::shared_ptr<BumperSensor> sensor_;
    float forwardSpeed_; // unit m/s
    float reverseSpeed_; // unit m/s
    float turningSpeed_; // unit deg/s
    float turnOnHitAngleDegree_;
    float reverseOnHitDurationSec_;
    PositionState positionState_;
    float stateDurationSec_; // -1 meaning keep the state unless a hit happnes
    float elapsedTimeOnStateSec_;
    float totalElapsedTimeSec_;

    const Cylinder& getShape() const { return *(Cylinder*)shape_.get(); }
};

} // namespace rosa