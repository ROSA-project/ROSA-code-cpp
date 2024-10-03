#include "ball.hpp"
#include "intersection_instance.hpp"


namespace rosa {

RigidPointBall::RigidPointBall(const ObjectId& oid,
                               const std::string& name,
                               std::unique_ptr<Sphere>&& shape,
                               const Position& position,
                               const std::shared_ptr<Object>& owner_object,
                               const std::shared_ptr<ObjectRegistry>& registry,
                               const Velocity& velocity,
                               float acceleration)
    : RigidPhysicalObject(oid,
                          name,
                          std::move(shape),
                          position,
                          owner_object,
                          registry,
                          velocity,
                          acceleration) {}

Position RigidPointBall::newPositionUponBump() {
    Position p;
    return p;

    //     // That is, we're here because infinitesimal intersections had occurred in the
    //     previous cycle.
    //     // TODO: assuming other objects are still, meaning that we have no change in
    //     momentum in other words we are not currently modelling bump of two moving
    //     objects

    //     for (auto &in_in: latestIntersections_) {
    //         if (!in_in->doesIntersect() || !in_in->isInfinitesimal()) {
    //             continue;
    //         }
    //         // TODO: I'm taking the first bump, process and return! we can have
    //         multiple bumps at the same time resulting in a combined change.

    //         // logger.Logger.add_line("processing a bump:")

    //         // TODO: we won't have this method. just an oversimplification for now
    //         bump_point expected to be a numpy array with 3 elements x, y and z auto&
    //         intersection_points = in_in->getIntersectionPoint();

    //         // TODO temporary: average two intersection points to find the bump point.
    //         bump_point = 0.5 * (intersection_points[0][0] + intersection_points[0][1])
    //         // logger.Logger.add_line("given bump point = " + str(bump_point))

    //         center_point = np.array([self.position.x, self.position.y,
    //                                     self.position.z])
    //         logger.Logger.add_line("circle (ball) center = " + str(center_point))

    //         new_phi, new_theta = RigidPointBall.calculate_circle_bounce(
    //             center_point, self.position.phi, self.position.theta, bump_point)

    //         new_position = copy.copy(self.position)
    //         new_position.phi = new_phi
    //         new_position.theta = new_theta
    //         logger.Logger.add_line("phi and theta changed from " + str(
    //             self.position.phi) + ", " + str(self.position.theta) + " to " +
    //                                 str(new_phi) + ", " + str(new_theta))
    //         logger.Logger.add_line("processing a bump finished.")
    //         return new_position
};

float RigidPointBall::getRequiredDeltaT() const {
    // TODO just some dummy value
    //return 2 * getShape().getRadius() / velocity_ / 200;
    return 2 * getShape().getRadius() / 200;
}

void RigidPointBall::calculateCircleBounce() {}

} // namespace rosa