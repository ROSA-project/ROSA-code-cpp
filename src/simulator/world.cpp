#include "world.hpp"
#include "common/constants.hpp"
#include "common/logger.hpp"
#include "common/util.hpp"
#include "config.hpp"
#include "map.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>

namespace rosa {

World::World(const std::string& map_filename,
             const std::string& vis_filename,
             const WorldConfig w_config)
    : numEvolutions_(0)
    , timeSinceStartMSec_(0)
    , visOutputFilename_(vis_filename)
    , durationMSec_(w_config.simDurationMsec) // 10 seconds
    , visFrameIntervalMsec_(w_config.visFrameIntervalMsec)
    , nextFrameTimeMsec_(0) {
    // TODO hardcoded parameters in this ctor, to be taken care of properly

    creationTs_ = time_since_epoch_in_millisec();

    // Parse input map
    Map map;
    registry_ = map.parseMap(map_filename);
    LOG_INFO("Successfully loaded the map from {}", map_filename);
}

void World::evolve(float delta_t) {
    auto& objs = registry_->getObjects();
    for (auto& p: objs) {
        if (p.second->isEvolvable()) {
            auto offspring_objects = p.second->evolve(delta_t);
            registry_->addObjects(offspring_objects);
            // TODO these newly added objects don't get an evolve() in this round?
        }
    }
    deleteExpiredObjects();
}

void World::deleteExpiredObjects() {
    auto& objects = registry_->getObjects();
    auto it = objects.begin();
    while (it != objects.end()) {
        if (it->second->timeToDie()) {
            // erase() returns the iterator to the next element
            it = objects.erase(it);
        } else {
            it++;
        }
    }
}

std::pair<World::InInType, bool> World::intersect() {
    InInType result;
    bool non_infinitesimal_intersect = false;
    auto& objects = registry_->getObjects();
    for (auto it1 = objects.begin(); it1 != objects.end(); it1++) {
        auto it2 = it1;
        std::advance(it2, 1);
        for (; it2 != objects.end(); it2++) {
            // TODO: Skipping shape-less objects, but shapelessness is not so
            // well-defined. let's return to this later
            if (it1->second->getShape().isShapeless()
                || it2->second->getShape().isShapeless()) {
                continue;
            }
            auto instance =
                new IntersectionInstance(*(it1->second.get()), *(it2->second.get()));
            std::shared_ptr<IntersectionInstance> in_in(instance);
            // instance has to be added for both objects
            result[it1->first].push_back(in_in);
            result[it2->first].push_back(in_in);
            if (in_in->doesIntersect()) {
                if (!in_in->isInfinitesimal()) {
                    // TODO: with the current architecture we could immediately break and
                    // return or throw an exception here. But that would be optimization.
                    // decide later.
                    non_infinitesimal_intersect = true;
                }
            }
        }
        // TODO: break here? this function may require a restructuring.
        break;
    }
    return std::make_pair(result, non_infinitesimal_intersect);
}

void World::registerIntersections(const World::InInType& intersection_result) {
    auto& objects = registry_->getObjects();
    for (auto& p: intersection_result) {
        objects[p.first]->setIntersections(p.second);
    }
}

float World::pickDeltaT() {
    // TODO: For now, we will only  run the world for one round
    // float new_dt = durationMSec_ + 1;
    // for (auto& p: registry_->getObjects()) {
    //     new_dt = std::min(new_dt, p.second->getRequiredDeltaT());
    // }
    // return new_dt;
    return 10;
}

void World::run() {
    // The json object containing data to be used by the visualizer
    nlohmann::json vis_json;

    float last_progress = 0;
    bool success = true;
    while (timeSinceStartMSec_ < durationMSec_) {
        auto delta_t = pickDeltaT();
        if (delta_t == 0) {
            LOG_ERROR("delta_t is set to 0.");
            success = false;
            break;
        }
        auto progress = (timeSinceStartMSec_ * 100ULL) / durationMSec_;
        if (progress > last_progress + 5) {
            last_progress = progress;
            LOG_INFO("Progress: {}%", progress);
        }

        // TODO: logger.Logger.add_line("at t = " + str(self.__current_time_ms) + ", "
        // "picked delta_t = " + str(delta_t))
        evolve(delta_t);

        auto [intersection_result, non_inf_intersect] = intersect();
        if (non_inf_intersect) {
            LOG_ERROR("Non-inf. intersection happened! Exiting simulation loop");
            success = false;
            break;
        }

        // Passes intersection result to the objects where the info will be used by object
        // to handle possible intersection consequences. (differentiate this from object
        // evolution)
        registerIntersections(intersection_result);

        updateVisualizationJson(vis_json);

        timeSinceStartMSec_ += delta_t;
        numEvolutions_ += 1;
    }
    LOG_DEBUG("Finished the simulation cycles.");

    dumpObjectInfo(vis_json);

    writeVisDataToFile(vis_json);

    if (success) {
        LOG_INFO("Simulation ended successfully!");
    } else {
        LOG_ERROR("Exiting the simulation with error!");
    }
}

void World::updateVisualizationJson(nlohmann::json& vis_json) {
    std::stringstream key_stream;
    while (timeSinceStartMSec_ >= nextFrameTimeMsec_) {
        key_stream << std::fixed << std::setprecision(VIZ_DECIMAL_LENGTH)
                   << nextFrameTimeMsec_;
        std::string key = key_stream.str();
        vis_json[key] = nlohmann::json({});

        // Populate with the positions of objects
        for (auto& p: registry_->getObjects()) {
            auto oid_str = std::to_string(p.first);
            vis_json[key][oid_str] = p.second->getPosition().toJson();
        }
        nextFrameTimeMsec_ += visFrameIntervalMsec_;
    }
}

void World::dumpObjectInfo(nlohmann::json& vis_json) {
    // Dump shapes and owners info.
    vis_json["shapes"] = nlohmann::json({});
    vis_json["owners"] = nlohmann::json({});

    for (auto& p: registry_->getObjects()) {
        auto oid_str = std::to_string(p.first);
        vis_json["shapes"][oid_str] = p.second->getShape().toJson();
        if (auto owner = p.second->getOwnerObject().lock()) {
            vis_json["owners"][oid_str] = std::to_string(owner->getObjectId());
        }
    }
    LOG_DEBUG("Dumped objects' info to vis json");
}

void World::writeVisDataToFile(nlohmann::json& vis_json) {
    std::ofstream o(visOutputFilename_);
    o << std::setw(4) << vis_json << std::endl;

    std::ofstream file(visOutputFilename_);
    if (config::prettified_json_output) {
        file << std::setw(4) << vis_json << std::endl;
    } else {
        file << vis_json;
    }
    LOG_DEBUG("Wrote vis json to file: {}", visOutputFilename_);
}

} // namespace rosa