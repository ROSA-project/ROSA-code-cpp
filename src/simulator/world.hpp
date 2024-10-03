#pragma once

// import time
#include "intersection_instance.hpp"
#include "nlohmann/json.hpp"
#include "object.hpp"
#include "object_registry.hpp"
#include "position.hpp"
#include <unordered_map>
#include <vector>

namespace rosa {

struct WorldConfig {
    uint64_t simDurationMsec;
    uint64_t visFrameIntervalMsec;
};

/**
 * Maintains the state of the world including all its objects. There will be only one
 * instance of this class, responsible for evolving the objects and runs the main cycle.
 */
class World {

    // type aliasing
    using InInType =
        std::unordered_map<ObjectId, std::vector<std::shared_ptr<IntersectionInstance>>>;

public:
    World(const std::string& map_filename,
          const std::string& vis_filename_prefix,
          const WorldConfig w_config);

    /**
     * World's main cycle.
     *
     * In each iteration, intersections of objects are computed and registered, and then
     * each object is evolved.
     */
    void run();

    ObjectId getTesterID(int);

    Position getObjectPosition(ObjectId);

private:
    // An instance of ObjectRegistry through which one can access list of all objects,
    std::shared_ptr<ObjectRegistry> registry_;

    // Timestamp of world creation.
    uint64_t creationTs_;

    // How many evolutions have the world had so far.
    uint64_t numEvolutions_;

    // Simulator's current time in millisecond.
    uint64_t timeSinceStartMSec_;

    const std::string visOutputFilenamePrefix_;

    // How many milli seconds the world instance will exist for.
    // 0 is unlimited.
    const uint64_t durationMSec_;

    // The time in msec between each two visualization frames.
    float visFrameIntervalMsec_;

    // The time in which the next frame will be taken
    float nextFrameTimeMsec_;

    /**
     * Transitions the objects into next state.
     *
     * @param delta_t Time difference between now and next state in seconds.
     */
    void evolve(float delta_t);

    /**
     * Removes objects which are supposed to die in this iteration.
     */
    void deleteExpiredObjects();

    /**
     * Computes the intersection of all pairs of objects.
     *
     * @return <A, b>, where A is the list of intersections, and b is whether a
     * non-infinitesimal intersection has occurred.
     */
    std::pair<InInType, bool> intersect();

    /**
     * For each object with intersections, registers the list of its intersections.
     *
     * @param intersection_result A map in which a list of IntersectionInstance objects is
     * stored per object
     */
    void registerIntersections(const InInType& intersection_result);

    /**
     * Returns a delta_t for the current cycle.
     */
    float pickDeltaT();

    void updateVisualizationJson(nlohmann::ordered_json& json);

    /**
     * Dump visualization info for shapes to the output json file
     */
    void dumpObjectInfo(nlohmann::ordered_json& vis_json);

    void writeVisDataToFile(nlohmann::ordered_json& vis_json, const std::string& vis_filename);
};

} // namespace rosa