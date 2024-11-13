#include "vacuum_cleaner.hpp"
#include "common/util.hpp"
#include <algorithm>

namespace rosa {

VacuumCleaner::VacuumCleaner(const ObjectId& oid,
                             const std::string& name,
                             std::unique_ptr<Cylinder>&& shape,
                             const Position& position,
                             const std::weak_ptr<Object>& owner_object,
                             const std::shared_ptr<ObjectRegistry>& registry)
    : Robot(oid,
            name,
            std::forward<std::unique_ptr<Cylinder>>(shape),
            position,
            owner_object,
            registry)
    , isInitialized_(false)
    , forwardSpeed_(1)
    , reverseSpeed_(0.2)
    , turningSpeed_(50)
    , turnOnHitAngleDegree_(50)
    , reverseOnHitDurationSec_(1)
    , positionState_(PositionState::FORWARD)
    , stateDurationSec_(-1) // -1 meaning keep the state unless a hit happnes
    , elapsedTimeOnStateSec_(0)
    , totalElapsedTimeSec_(0) {}

void VacuumCleaner::initializeSensor() {
    auto sensor_id = registry_->getNextAvailableId();
    auto sensor_shape = std::make_unique<Cylinder>(getShape());
    sensor_ = std::make_shared<BumperSensor>(sensor_id,
                                             name_,
                                             std::move(sensor_shape),
                                             position_,
                                             weak_from_this(),
                                             registry_);
    registry_->addObjects({{sensor_id, sensor_}});
    isInitialized_ = true;
}

Object::ObjectMap VacuumCleaner::evolve(float delta_t) {
    if (sensor_->sense()) {
        // A hit occured.
        // State (position) is reverted by Object. Here we just make a decision about what
        // to do upon bump
        elapsedTimeOnStateSec_ = 0;
        switch (positionState_) {
        case PositionState::FORWARD: {
            positionState_ = PositionState::TURN_LEFT;
            stateDurationSec_ = turnOnHitAngleDegree_ / turningSpeed_;
            break;
        }
        case PositionState::TURN_LEFT: {
            positionState_ = PositionState::REVERSE;
            stateDurationSec_ = reverseOnHitDurationSec_;
            break;
        }
        case PositionState::REVERSE: {
            positionState_ = PositionState::TURN_LEFT;
            stateDurationSec_ = turnOnHitAngleDegree_ / turningSpeed_;
            break;
        }
        default:
            rosa_assert(1 == 2, "Invalid position state");
        }
    }

    else {
        // No hit so no unexpcted change of state
        elapsedTimeOnStateSec_ += delta_t;
        auto new_position = position_;
        // if (stateDurationSec_ != -1) {
        //     // We're in a duration-limited state
        //     if (elapsedTimeOnStateSec_ >= stateDurationSec_) {
        //         // Time to go back to normal operation of forward
        //         positionState_ = PositionState::FORWARD;
        //         stateDurationSec_ = -1;
        //     }
        // }
        // switch (positionState_) {
        // case PositionState::FORWARD: {
        //     auto distance = forwardSpeed_ * delta_t;
        //     // That is when oriented toward left we get decreasing x
        //     new_position.x += distance * cos(PI_CONST / 180 * position_.phi);
        //     // That is when oriented toward buttom we get decreasing y
        //     new_position.y += distance * sin(PI_CONST / 180 * position_.phi);
        //     break;
        // }
        // case PositionState::TURN_LEFT: {
        //     // Left meaning increasing phi
        //     auto rotation = turningSpeed_ * delta_t; // in degrees
        //     new_position.phi += rotation;
        //     break;
        // }
        // case PositionState::REVERSE: {
        //     if (elapsedTimeOnStateSec_ == 0) {
        //         // Reversing not began yet, reverse the direction
        //         new_position.phi = -position_.phi;
        //     }
        //     auto distance = reverseSpeed_ * delta_t;
        //     // That is when oriented toward left we get decreasing x
        //     new_position.x += distance * cos(PI_CONST / 180 * position_.phi);
        //     // That is when oriented toward buttom we get decreasing y
        //     new_position.y += distance * sin(PI_CONST / 180 * position_.phi);
        //     break;
        // }
       // default:
         //   rosa_assert(1 == 2, "Invalid position state");
        
        updatePosition(new_position);
        sensor_->updatePosition(new_position);

    // print("x=" + str(self.position.x) + " ," + "y=" + str(self.position.y) + " ," + \
    //    "phi=" + str(self.position.phi))
    }
    totalElapsedTimeSec_ += delta_t;
    return {};
}
float VacuumCleaner::getRequiredDeltaT() const {
    // Say, enough to capture turning or reversing duration in 10 cycles
    return std::min(turnOnHitAngleDegree_ / turningSpeed_, reverseOnHitDurationSec_) / 10;
}

} // namespace rosa