#include "world.hpp"
#include "map.hpp"
#include "utils/constants.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>

namespace rosa {

World::World(const std::string& map_filename, const std::string& vis_filename)
    : numEvolutions_(0)
    , currentTime_(std::time(0))
    , visOutputFilename_(vis_filename)
    , durationSec_(10)
    , visFrameIntervalMsec_(0.025)
    , nextFrameTimeMsec_(0) {
    // TODO hardcoded parameters in this ctor, to be taken care of properly

    // Parse input map
    Map map;
    registry_ = map.parseMap(map_filename);
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
    float new_dt = durationSec_ + 1;
    for (auto& p: registry_->getObjects()) {
        new_dt = std::min(new_dt, p.second->getRequiredDeltaT());
    }
    return new_dt;
}

void World::run() {
    // The json object containing data to be used by the visualizer
    nlohmann::json vis_json;

    float last_progress = 0;
    while (currentTime_ < durationSec_) {
        auto delta_t = pickDeltaT();
        auto progress = currentTime_ / durationSec_ * 100;
        if (progress > last_progress) {
            last_progress = progress;
            // TODO: print(str(current_percentage) + "% processed")
        }

        // TODO: logger.Logger.add_line("at t = " + str(self.__current_time_ms) + ", "
        // "picked delta_t = " + str(delta_t))
        evolve(delta_t);
        auto [intersection_result, non_inf_intersect] = intersect();
        if (non_inf_intersect) {
            // TODO: print("non-infinitesimal_intersection happened! exiting simulation
            // loop")
            break;
        }

        // Passes intersection result to the objects where the info will be used by object
        // to handle possible intersection consequences. (differentiate this from object
        // evolution)
        registerIntersections(intersection_result);

        updateVisualizationJson(vis_json);

        currentTime_ += delta_t;
        numEvolutions_ += 1;
    }

    dumpObjectInfo(vis_json);
    writeVisDataToFile(vis_json);
}

void World::updateVisualizationJson(nlohmann::json& vis_json) {
    std::stringstream key_stream;
    while (currentTime_ >= nextFrameTimeMsec_) {
        key_stream << std::fixed << std::setprecision(VIZ_DECIMAL_LENGTH)
                   << nextFrameTimeMsec_;
        std::string key = key_stream.str();
        vis_json[key] = nlohmann::json({});

        // Populate with the positions of objects
        for (auto& p: registry_->getObjects()) {
            vis_json[key][p.first] = p.second->getPosition().toJson();
        }
        nextFrameTimeMsec_ += visFrameIntervalMsec_;
    }
}

void World::dumpObjectInfo(nlohmann::json& vis_json) {
    // Dump shapes and owners info.
    vis_json["shapes"] = nlohmann::json({});
    vis_json["owners"] = nlohmann::json({});
    for (auto& p: registry_->getObjects()) {
        vis_json["shapes"][p.first] = p.second->getShape().toJson();
        if (auto owner = p.second->getOwnerObject().lock()) {
            vis_json["owners"][p.first] = std::to_string(owner->getObjectId());
        }
    }
}

void World::writeVisDataToFile(nlohmann::json& vis_json) {
    std::ofstream file(visOutputFilename_);
    file << vis_json;
}

} // namespace rosa