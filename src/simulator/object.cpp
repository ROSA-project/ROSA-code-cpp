#include "object.hpp"
#include "common/logger.hpp"
#include "intersection_instance.hpp"
#include "object_registry.hpp"

namespace rosa {

Object::Object(const ObjectId& oid,
               const std::string& name,
               std::unique_ptr<Shape>&& shape,
               const Position& position,
               const std::weak_ptr<Object>& owner_object,
               const std::shared_ptr<ObjectRegistry>& registry)
    : oid_(oid)
    , name_(name)
    , shape_(std::move(shape))
    , position_(position)
    , previousPosition_(position)
    , ownerObject_(owner_object)
    , registry_(registry)
    , infinitesimalIntersectionOccured_(false)
    , evolvable_(ownerObject_.lock() ? false : true) {}

void Object::setOwner(const std::weak_ptr<Object>& owner_object) {
    ownerObject_ = owner_object;
}

void Object::addDependentObject(const std::shared_ptr<Object>& obj) {
    dependentObjects_[obj->oid_] = obj;
}

Object::ObjectMap Object::evolve(float delta_t) {
    // TODO: evolve should access to the list of intersections in this iteration.
    // TODO: evolve should actually change the state. We currently do not change
    // Position or Speed
    ObjectMap offspring_objects;
    for (auto& p: dependentObjects_) {
        auto offsprings = p.second->evolve(delta_t);
        offspring_objects.insert(offsprings.begin(), offsprings.end());
    }
    return offspring_objects;
}

void Object::setIntersections(
    const std::vector<std::shared_ptr<IntersectionInstance>>& intersections) {
    // TODO: do we need to copy the intersections here?
    latestIntersections_ = intersections;
    infinitesimalIntersectionOccured_ = false;
    std::vector<double> revert(3,1);
    for (auto& in_in: latestIntersections_) {
        if (in_in->doesIntersect() && in_in->isInfinitesimal()) {
            infinitesimalIntersectionOccured_ = true;
           auto [rev1, rev2] = in_in->reversion();
           revert = (rev1[3] == oid_) ? rev1 : rev2;
        }
    }

    if (infinitesimalIntersectionOccured_) {
        infinitesimalIntersectionImmediate(revert);
    }
}

void Object::infinitesimalIntersectionImmediate(const std::vector<double>& revert) {
    // logger.Logger.add_line("infinitestimal intersection detected, reverting position
    // (default Object behavior)")
    revertPosition(revert);
}

void Object::updatePosition(const Position& new_position) {
    previousPosition_ = position_;
    position_ = new_position;
}

void Object::revertPosition(const std::vector<double>& revert) {
    // TODO leaves the position and previous position the same.
    // better to somehow invalidate previous position? (same should happen in
    // constructor where these two are again the same)
    position_.x = revert[0];
    position_.y = revert[1];
    position_.z = revert[2];
}

float Object::getRequiredDeltaT() const { return 0; }

bool Object::timeToDie() const { return false; }

bool Object::isEvolvable() const { return evolvable_; }

// TODO const or something?
Position Object::getPosition() { return position_; }

} // namespace rosa