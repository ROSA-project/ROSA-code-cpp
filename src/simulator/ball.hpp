#pragma once

#include "cylinder.hpp"
#include "sphere.hpp"
#include "position.hpp"
#include "rigid_physical_obj.hpp"
#include "velocity.hpp"

namespace rosa {

/**
 * A rigid ball that reflects from the bump surface. It is a point object as it does not
 * rotate
 */
class RigidPointBall : public RigidPhysicalObject {
public:
    RigidPointBall(const ObjectId& oid,
                   const std::string& name,
                   std::unique_ptr<Sphere>&& shape,
                   const Position& position,
                   const std::shared_ptr<Object>& owner_object,
                   const std::shared_ptr<ObjectRegistry>& registry,
                   const Velocity& velocity ,
                   float acceleration);

    /**
     * Handles the new direction of movement when a bump has happened.
     * @return The position carrying new orientation. i.e. the cartesian part is not used.
     */
    Position newPositionUponBump() override;

    /**
     * Returns the delta_t that this object requires to operate right. returns 0 if the
     * objects declares no requirement.
     */
    float getRequiredDeltaT() const override;

private:
    void calculateCircleBounce();

    const Cylinder& getShape() const { return *(Cylinder*)shape_.get(); }
};

} // namespace rosa