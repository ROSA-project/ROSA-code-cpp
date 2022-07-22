#pragma once

// import time
#include "intersection_instance.hpp"
#include "object.hpp"
#include "object_registry.hpp"
#include "position.hpp"

#include <unordered_map>
#include <vector>

namespace rosa {

/**
 * Maintains the state of the world including all its objects. There will be only one
 * instance of this class, responsible for evolving the objects and runs the main cycle.
 */
class World {

// type aliasing
using InInType = std::unordered_map<ObjectId, std::vector<std::shared_ptr<IntersectionInstance> > >;

public:
    World(const std::string& map_filename, const std::string& vis_filename);

    /**
     * @brief World's main cycle.
     * 
     * In each iteration, intersections of objects are computed and registered, and then
     * each object is evolved.
     */
    void run();

private:
    // An instance of ObjectRegistry through which one can access list of all objects,
    std::shared_ptr<ObjectRegistry> registry_;

    // Timestamp of world creation.
    uint64_t creationTs_;

    // How many evolutions have the world had so far.
    uint64_t numEvolutions_;

    // Simulator's current time in millisecond.
    uint64_t currentTime_;

    const std::string visOutputFilename_;

    // How many seconds the world instance will exist for.
    const uint64_t durationSec_;

    // The time in msec between each two visualization frames.
    float visFrameIntervalMsec_;
    
    // The time in which the next frame will be taken
    float nextFrameTimeMsec_;


    /**
     * @brief Transitions the objects into next state.
     * 
     * @param delta_t Time difference between now and next state in seconds. 
     */
    void evolve(float delta_t);

    /**
     * @brief Removes objects which are supposed to die in this iteration.
     */
    void deleteExpiredObjects();

    /**
     * @brief Computes the intersection of all pairs of objects.
     * 
     * @return <A, b>, where A is the list of intersections, and b is whether a non-infinitesimal intersection has occurred. 
     */
    std::pair<InInType, bool> intersect();

    /**
     * @brief For each object with intersections, registers the list of its intersections.
     * 
     * @param intersection_result A map in which a list of IntersectionInstance objects is stored per object 
     */
    void registerIntersections(const InInType& intersection_result);

    /**
     * @brief Returns a delta_t for the current cycle.
     */
    float pickDeltaT();

        void updateVisualizationJson();

    /**
     * @brief Dump visualization info for shapes to the output json file
     */
    void dumpAllShapesInfo();

    void dumpVisDataToFile();

    /**
     * @brief Sets position of the objects with moment (time).
     * 
     * @return A map with ObjectID as key, and object's visualization info as value.
     */
    std::unordered_map<ObjectId, VisInfo> visualize();

    /**
     * @brief Dump visualization info for Owners between objects to the output json file.
     * 
     */
    void dumpAllOwnersInfo();
};

} // namespace rosa