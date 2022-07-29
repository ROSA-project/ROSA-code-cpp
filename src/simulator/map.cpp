#include "map.hpp"

#include "ball.hpp"
#include "box.hpp"
#include "cube.hpp"
#include "cylinder.hpp"
#include "vacuum_cleaner.hpp"
#include "utils/util.hpp"
#include <fstream>
#include <stdexcept>

namespace rosa {

Map::Map()
: nextAvailableId_(0) {
}

std::shared_ptr<ObjectRegistry> Map::parseMap(const std::string& filename) {
    std::ifstream ifs(filename);
    if (ifs) {
        // file was openned successfully.
            nlohmann::json json = nlohmann::json::parse(ifs);
            std::shared_ptr<ObjectRegistry> registry;
            std::unordered_map<ObjectId, std::shared_ptr<Object>> new_objects;
            getObjects(registry, new_objects, json, nullptr);
            registry->addObjects(new_objects);
            return registry;
    }
    else {
        // TODO: print sth on the logger
        throw std::invalid_argument("Error in opening file: " + filename);
    }
}

const Object::ObjectMap& Map::getObjects(std::shared_ptr<ObjectRegistry> registry, Object::ObjectMap& obj_map, const nlohmann::json &json, std::shared_ptr<Object> owner) {
    // this_level_objects = {};
    // for (auto oname: json) {
    //     auto new_obj_id = registry->getNextAvailableId();
    //     obj = instantiate_object(parsed[oname], new_obj_id, oname, owner)
    //     this_level_objects[new_id] = obj
    //     if "subobjects" in parsed[oname]:
    //         assert parsed[oname]["class"] == "CompoundPhysical", \
    //             "Only CompoundPhysical objects can have subobjects"
    //         obj.dependent_objects = getObjects(obj_map, parsed[oname]["subobjects"], obj)
    //     else:
    //         obj.dependent_objects = {}

    //     obj_map[new_id] = obj
    // return this_level_objects
}

std::shared_ptr<Object> Map::instantiateObject(std::shared_ptr<ObjectRegistry> registry, const nlohmann::json &json, ObjectId new_id, const std::string& name, std::shared_ptr<Object> owner) {
    auto shape = getShape(json);
    if (shape == nullptr) {
        rosa_assert(json["class"] == "CompoundPhysical", "Only CompoundPhysical objects can be shape-less");
    }
    auto position = getPosition(json);
    auto cname = json["class"].get<std::string>();
    if (cname == "Box") {
        auto u_ptr = std::unique_ptr<Cube>((Cube*)shape);
        return std::make_shared<Box>(new_id, name, std::move(u_ptr), position, owner, registry_);
    }
    else if (cname == "RigidPointBall") {
        // TODO hardcoding acceleration and velocity not to change 
        // Erfan's code w/o discussion
        auto u_ptr = std::unique_ptr<Cylinder>((Cylinder*)shape);
        return std::make_shared<RigidPointBall>(new_id, name, std::move(u_ptr), position, owner, registry_, 0, 2);
    }   
    else if (cname == "VacuumCleanerV0") {
        auto u_ptr = std::unique_ptr<Cylinder>((Cylinder*)shape);
        //std::make_unique<Cylinder>(std::stod(params.at("diameter")), std::stod(params.at("height"))),
        return std::make_shared<VacuumCleaner>(new_id, name, std::move(u_ptr), position, owner, registry_);
    }
    // TODO:
    // else if (cname == "Simple" || cname == "CompoundPhysical") {
    //     return std::make_shared<Object>(new_id, name, shape, position, owner, registry_);
    // }

    rosa_assert(1==2, "Unknown 'class' name for object: {cname}");
    return std::shared_ptr<Object>(nullptr);    
}

Shape* Map::getShape(const nlohmann::json &json) {
    return nullptr;
    // if ()
    //  "shape" not in obj_json:
    //     return None
    // else:
    //     try:
    //         args = obj_json["shape"]["arguments"]
    //         cname = obj_json["shape"]["class"]
    //         if cname == "Cube":
    //             return Cube(args["lenght"], args["height"], args["width"])
    //         elif cname == "Cylinder":
    //             return Cylinder(args["radius"], args["height"])
    //         else:
    //             raise ValueError("Unknown shape: ", cname)
    //     except Exception as e:
    //         print("Error in parsing object's shape: ", str(e))
    //         raise e
}

Position Map::getPosition(const nlohmann::json &json) {
    return Position();

}






} // namespace rosa