#pragma once 

#include "cylinder.hpp"
#include "position.hpp"
#include "rigid_physical_obj.hpp"

namespace rosa {

/**
 * A rigid ball that reflects from the bump surface. It is a point object as it does not rotate
 */
class RigidPointBall: public RigidPhysicalObject {
public:
    RigidPointBall(const ObjectId& oid,
                        const std::string& name,
                        std::unique_ptr<Cylinder>&& shape,
                        const Position& position,
                        const std::shared_ptr<Object>& owner_object,
                        const std::shared_ptr<ObjectRegistry>& registry,
                        float acceleration,
                        float velocity);
    
    /**
     * Handles the new direction of movement when a bump has happened.
     * @return The position carrying new orientation. i.e. the cartesian part is not used.
     */
    Position newPositionUponBump() override;

    /**
     * Returns the delta_t that this object requires to operate right. returns 0 if the objects declares no requirement.
     */
    float getRequiredDeltaT() const override;

private:
    void calculateCircleBounce();

    const Cylinder& getShape() const { *(Cylinder*)shape_.get(); }
};

} // namespace rosa