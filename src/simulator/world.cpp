#include "world.hpp"
#include <ctime>

namespace rosa {

World::World(const std::string& map_filename, const std::string& vis_filename)
: numEvolutions_(0)
, currentTime_(std::time(0))
, visOutputFilename_(vis_filename)
, durationSec_(10)
, visFrameIntervalMsec_(0.025)
, nextFrameTimeMsec_(0) {
    // TODO hardcoded parameters in this ctor, to be taken care of properly

    registry_ = std::make_shared<ObjectRegistry>();

    // Parse input map
    map_ = std::make_shared<Map>(registry_);
    map_.parse_map(map_filename);
}

void World::evolve(float delta_t) {
    auto &objs = registry_->getObjects();
    for (auto &p: objs) {
        if (p.second->isEvolvable()) {
            auto &offspring_objects = p.second->evolve(delta_t);
            registry_->addObjects(offspring_objects);
            // TODO these newly added objects don't get an evolve() in this round?
        }
    }
    deleteExpiredObjects();
}


void World::deleteExpiredObjects() {
    auto &objects = registry_->getObjects();
    auto it = objects.begin();
    while (it != objects.end()) {
        if (it->second->timeToDie()) {
            // erase() returns the iterator to the next element
            it = objects.erase(it);
        }
        else {
            it++;
        }
    }
}

std::pair<World::InInType, bool> World::intersect() {
    InInType result;
    bool non_infinitesimal_intersect = false;
    auto &objects = registry_->getObjects();
    for (auto it1 = objects.begin(); it1 != objects.end(); it1++) {
        auto it2 = it1;
        std::advance(it2, 1);
        for (; it2 != objects.end(); it2++) {
            // TODO: Skipping shape-less objects, but shapelessness is not so well-defined.
            // let's return to this later
            if (it1->second->getShape().isShapeless() || it2->second->getShape().isShapeless()) {
                continue;
            }
            auto instance = new IntersectionInstance( *(it1->second.get()), *(it2->second.get()) );
            std::shared_ptr<IntersectionInstance> in_in(instance);
            // instance has to be added for both objects
            result[it1->first].push_back(in_in);
            result[it2->first].push_back(in_in);
            if (in_in->doesIntersect()) {
                if (!in_in->isInfinitesimal()) {
                    // TODO: with the current architecture we could immediately break and return
                    // or throw an exception here. But that would be optimization. decide later.
                    non_infinitesimal_intersect = true;
                }
            }
        }
        return std::make_pair(result, non_infinitesimal_intersect);
    }
}

void World::registerIntersections(const World::InInType& intersection_result) {
    auto &objects = registry_->getObjects();
    for (auto &p: intersection_result) {
        objects[p.first].setIntersections(p.second);
    }
}


// def pick_delta_t(self) -> float:
//     """Returns a delta_t for the current cycle
//     """
//     # TODO: For now, we will only  run the world for one round
//     delta_t_list = [self.__duration_sec + 1]
//     for oid in self.registry.get_objects():
//         delta_t_list.append(self.registry.get_objects()[oid].get_required_delta_t())
//     return float(min(set(delta_t_list) - {0}))

// def run(self) -> None:
//     """World's main cycle.

//     In each iteration, intersections of objects are computed and registered, and then
//     each object is evolved.
//     """
//     non_infinitesimal_intersection_exists: bool = False
//     current_percentage = 0
//     while self.__current_time_ms < self.__duration_sec:
//         delta_t = self.pick_delta_t()
//         if int(self.__current_time_ms / self.__duration_sec * 100) > current_percentage:
//             current_percentage = int(self.__current_time_ms / self.__duration_sec * 100)
//             print(str(current_percentage) + "% processed")
//         logger.Logger.add_line("at t = " + str(self.__current_time_ms) + ", "
//                                                                             "picked delta_t = " + str(delta_t))
//         self.evolve(delta_t)
//         intersection_result, non_infinitesimal_intersection_exists = self.intersect()
//         # TODO I don't want to assert so I can see the output :D assert kills the wrapper
//         # assert(not non_infinitesimal_intersection_exists)
//         if non_infinitesimal_intersection_exists:
//             print("non-infinitesimal_intersection happened! exiting simulation loop")
//             break

//         # passes intersection result to the objects
//         # where the info will be used by object to handle possible intersection
//         # consequences. (differentiate this from object evolution)
//         self.register_intersections(intersection_result)

//         self.update_visualization_json()

//         self.__current_time_ms = self.__current_time_ms + delta_t
//         self.__num_evolutions += 1

//     self.dump_all_shapes_info()
//     self.dump_all_owners_info()
//     self.dump_vis_data_to_file()

// def update_visualization_json(self):
//     step_round = 3
//     while self.__current_time_ms >= self.__next_frame_time_msec:
//         key = ("{:." + str(step_round) + "f}").format(self.__next_frame_time_msec)
//         self.__vis_data[key] = self.visualize()
//         self.__next_frame_time_msec += self.__vis_frame_interval_ms

// def dump_all_shapes_info(self):
//     # dump visualization info for shapes to the output json file
//     shapes_info_dict = {"shapes": {}}
//     for oid in self.registry.get_objects():
//         shapes_info_dict["shapes"][oid] = self.registry.get_objects()[oid].dump_shape_info()
//     self.__vis_data.update(shapes_info_dict)

// def dump_vis_data_to_file(self):
//     try:
//         with open(self.__vis_output_filename, "w") as json_file:
//             json.dump(self.__vis_data, json_file, indent=2)
//     except (OSError, IOError) as e:
//         print("Error in writing to file ", self.__vis_output_filename)
//         raise e

// def visualize(self) -> dict:
//     """Sets position of the objects with moment (time)
    
//     Returns:
//         Dictionary with ObjectID as key, and object's visualization info as value.
//     """
//     objects_info = dict()
//     for oid in self.registry.get_objects():
//         # TODO:In higher versions, this should be changed (if the visualize method is changed)
//         objects_info[oid] = self.registry.get_objects()[oid].visualize()
//     return objects_info

// def dump_all_owners_info(self) -> None:
//     """ dump visualization info for Owners between objects to the output json file

//     """
//     owners_info = {"owners": {}}
//     for oid in self.registry.get_objects():
//         if self.registry.get_objects()[oid].owner_object is not None:
//             owners_info["owners"][oid] = str(self.registry.get_objects()[oid].owner_object.oid)
//     self.__vis_data.update(owners_info)


} // namespace rosa