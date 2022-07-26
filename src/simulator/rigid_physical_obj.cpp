#include "rigid_physical_obj.hpp"
#include "common/constants.hpp" // PI_CONST
#include <cmath>

namespace rosa {

RigidPhysicalObject::RigidPhysicalObject(const ObjectId& oid,
                                         const std::string& name,
                                         std::unique_ptr<Shape>&& shape,
                                         const Position& position,
                                         const std::weak_ptr<Object>& owner_object,
                                         const std::shared_ptr<ObjectRegistry>& registry,
                                         float acceleration,
                                         float velocity)
    : Object(oid, name, std::move(shape), position, owner_object, registry)
    , acceleration_(acceleration)
    , velocity_(velocity) {}

Object::ObjectMap RigidPhysicalObject::evolve(float delta_t) {
    Position new_position;
    if (infinitesimalIntersectionOccured_) {
        // This is a bump which affects object's movement in this cycle
        // logger.Logger.add_line("RigidPhysicalObject infinitestimal intersection, "
        //                        "redirecting to update_state_upon_bump")
        new_position = newPositionUponBump();
    } else {
        new_position = position_;
    }

    auto r = velocity_ * delta_t;
    new_position.x += r * cos(new_position.phi * PI_CONST / 180);
    new_position.y += r * sin(new_position.phi * PI_CONST / 180);
    updatePosition(new_position);

    return {};
}

} // namespace rosa