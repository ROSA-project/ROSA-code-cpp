#pragma once

#include "object.hpp"
#include "object_registry.hpp"
#include "position.hpp"
#include "shape.hpp"
#include "velocity.hpp"

namespace rosa {

/**
 * A general rigid physical object, i.e. does not change shape by assumption, and is
 * evolved as if it is a point with no shape. Hence, e.g. no rotation. The object has
 * acceleration and velocity in addition to the position.
 */
class RigidPhysicalObject : public Object {
public:
    RigidPhysicalObject(const ObjectId& oid,
                        const std::string& name,
                        std::unique_ptr<Shape>&& shape,
                        const Position& position,
                        const std::weak_ptr<Object>& owner_object,
                        const std::shared_ptr<ObjectRegistry>& registry,
                        const Velocity& velocity , 
                        float acceleration);

    /**
     * Overriding that of Object. A RigidPhysicalObject basically keeps moving based on
     acceleration and velocity and changes state upon bumps.
     * The handling of the bump is implemented in children.
     *
     * @param delta_t Span of evolution in seconds
     * @return A map of all the new objects which are created by this object.
        These new objects will be added to the list of dependent objects.
     */
    virtual ObjectMap evolve(float delta_t) override;

    /**
     * Calculates the new motion state.
     *
     * TODO: for v0 I'm just assuming a bump impacts the direction
     * this will change later with bump of moving objects and introduction of momentum
     */
    virtual Position newPositionUponBump() = 0;

    virtual float getRequiredDeltaT() const override = 0;

protected:
    float acceleration_;
    Velocity velocity_;
};

} // namespace rosa