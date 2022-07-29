#pragma once

namespace rosa {
using ObjectId = int; // type aliasing, since we might change ObjectId composition
}

#include "position.hpp"
#include "shape.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace rosa {

class ObjectRegistry;
class IntersectionInstance;

class Object {
public:
    using ObjectMap = std::unordered_map<ObjectId, std::shared_ptr<Object>>;

    Object(const ObjectId& oid,
           const std::string& name,
           std::unique_ptr<Shape>&& shape,
           const Position& position,
           const std::weak_ptr<Object>& owner_object,
           const std::shared_ptr<ObjectRegistry>& registry);

    virtual ~Object() = default;

    const ObjectId getObjectId() const { return oid_; }

    const std::string& getName() const { return name_; }

    const Shape& getShape() const { return *shape_.get(); }

    const Position& getPosition() const { return position_; }

    const std::weak_ptr<Object> getOwnerObject() const { return ownerObject_; }

    /**
     * Sets the Owner object, in case this information was not available at the time of
     * construction.
     */
    void setOwner(const std::weak_ptr<Object>& owner_object);

    /**
     * Adds the given object to the list of world's objects.
     *
     * @param obj Object to be added.
     */
    void addDependentObject(const std::shared_ptr<Object>& obj);

    /**
     * Transitions the object and all its dependents to the next state.
     *
     * @param delta_t Span of evolution in seconds.
     * @return A map of all the new objects which are created by this object. These new
     * objects will be added to the list of dependent objects.
     */
    virtual ObjectMap evolve(float delta_t);

    /**
     * Registers this round's intersections. If any infinitesimal intersection, calls
     * handle_infinitesimal_intersection to decide on the consequences in the current
     * evolution cycle note that the consequences on behavior of the robot will be handled
     * in the next evolution cycle
     *
     * @param intersections List of this round's intersections
     */
    void setIntersections(
        const std::vector<std::shared_ptr<IntersectionInstance>>& intersections);

    /**
     * By default, we revert the position without reverting the rest of the state
     */
    void infinitesimalIntersectionImmediate();

    /**
     * All changes to the position must go through this function.
     * This is to ensure we keep the state. (# TODO how to enforce this?)
     */
    void updatePosition(const Position& new_position);

    /**
     * TODO: leaves the position and previous position the same.
     * better to somehow invalidate previous position? (same should happen in
     * constructor where these two are again the same)
     */
    void revertPosition();

    // def visualize(self) -> list:
    //     """
    //     """
    //     position = [self.position.x, self.position.y ,self.position.phi]
    //     return position

    // def dump_shape_info(self):
    //     if self.shape is None:
    //         return None
    //     return self.shape.dump_info()

    virtual std::shared_ptr<Box> boundingBox() { return std::shared_ptr<Box>(nullptr); }

    /**
     * Returns the delta_t that this object requires to operate right.
     * returns 0 if the objects declares no requirement.
     */
    virtual float getRequiredDeltaT() const;

    /**
     * Checks if object must cease to exist in the next iteration of the world.
     */
    bool timeToDie() const;

    /**
     * Checks if object should be evolved directly by the world.
     */
    bool isEvolvable() const;

protected:
    ObjectId oid_;
    const std::string name_;
    std::unique_ptr<Shape> shape_;
    Position position_;
    Position previousPosition_;
    // weak_ptr to avoid reference cycles (having both parent to child, and child to
    // parent references), which may result in memory leak.
    std::weak_ptr<Object> ownerObject_;
    std::shared_ptr<ObjectRegistry> registry_;
    std::unordered_map<ObjectId, std::shared_ptr<Object>> dependentObjects_;
    std::vector<std::shared_ptr<IntersectionInstance>> latestIntersections_;
    bool infinitesimalIntersectionOccured_;
    // Object is evolvable only if it has no owner.
    bool evolvable_;
};

} // namespace rosa