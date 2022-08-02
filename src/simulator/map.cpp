#include "map.hpp"

#include "ball.hpp"
#include "box.hpp"
#include "cube.hpp"
#include "cylinder.hpp"
#include "shapeless.hpp"
#include "common/util.hpp"
#include "vacuum_cleaner.hpp"

#include "spdlog/spdlog.h"

#include <fstream>
#include <stdexcept>

namespace rosa {

Map::Map(){}

std::shared_ptr<ObjectRegistry> Map::parseMap(const std::string& filename) {
    std::ifstream ifs(filename);
    if (ifs) {
        // file was openned successfully.
        nlohmann::json json = nlohmann::json::parse(ifs);
        auto registry = std::make_shared<ObjectRegistry>();
        std::unordered_map<ObjectId, std::shared_ptr<Object>> new_objects;
        getObjects(registry, new_objects, json, nullptr);
        registry->addObjects(new_objects);
        return registry;
    } else {
        // TODO: print sth on the logger
        throw std::invalid_argument("Error in opening file: " + filename);
    }
}

Object::ObjectMap Map::getObjects(std::shared_ptr<ObjectRegistry> registry,
                                         Object::ObjectMap& obj_map,
                                         const nlohmann::json& json,
                                         std::shared_ptr<Object> owner) {
    spdlog::info("map 0");
    Object::ObjectMap this_level_objects;
    for (auto& [oname, obj_json] : json.items()) {
        spdlog::info("map 1 oname {}, json {}", oname, obj_json.dump());

        auto obj = instantiateObject(registry, obj_json, oname, owner);
        spdlog::info("map 3");

        this_level_objects[obj->getObjectId()] = obj;
        spdlog::info("map 4");

        if (obj_json.find("subobjects") != obj_json.end()) {
            spdlog::info("map 5");

            rosa_assert(obj_json["class"].get<std::string>() == "CompoundPhysical", "Only CompoundPhysical objects can have subobjects");
            auto dependents = getObjects(registry, obj_map, obj_json["subobjects"], obj);
            spdlog::info("map 6");

            // Add all dependent objects
            for (auto &p: dependents) {
                spdlog::info("map 7");

                obj->addDependentObject(p.second);
            }
            spdlog::info("map 8");

        }
        spdlog::info("map 9");

        obj_map[obj->getObjectId()] = obj;
        spdlog::info("map 10");

    }
    

    return this_level_objects;
}

std::shared_ptr<Object> Map::instantiateObject(std::shared_ptr<ObjectRegistry> registry,
                                               const nlohmann::json& json,
                                               const std::string& name,
                                               std::shared_ptr<Object> owner) {
    spdlog::info("instantiate 1");
    auto new_id = registry->getNextAvailableId();
spdlog::info("instantiate 2");
    auto shape = getShape(json);
    spdlog::info("instantiate 3");
    if (shape == nullptr) {
        rosa_assert(json["class"] == "CompoundPhysical",
                    "Only CompoundPhysical objects can be shape-less");
    }
    auto position = getPosition(json);
    auto cname = json["class"].get<std::string>();
    if (cname == "Box") {
        auto u_ptr = std::unique_ptr<Cube>((Cube*)shape);
        return std::make_shared<Box>(
            new_id, name, std::move(u_ptr), position, owner, registry_);
    } else if (cname == "RigidPointBall") {
        // TODO hardcoding acceleration and velocity not to change
        // Erfan's code w/o discussion
        auto u_ptr = std::unique_ptr<Cylinder>((Cylinder*)shape);
        return std::make_shared<RigidPointBall>(
            new_id, name, std::move(u_ptr), position, owner, registry_, 0, 2);
    } else if (cname == "VacuumCleanerV0") {
        auto u_ptr = std::unique_ptr<Cylinder>((Cylinder*)shape);
        // std::make_unique<Cylinder>(std::stod(params.at("diameter")),
        // std::stod(params.at("height"))),
        return std::make_shared<VacuumCleaner>(
            new_id, name, std::move(u_ptr), position, owner, registry_);
    } else if (cname == "CompoundPhysical") {
        auto u_ptr = std::unique_ptr<Shapeless>((Shapeless*)shape);
        return std::make_shared<Object>(
            new_id, name, std::move(u_ptr), position, owner, registry_);
    } else if (cname == "Simple") {
        auto u_ptr = std::unique_ptr<Shape>(shape);
        return std::make_shared<Object>(
            new_id, name, std::move(u_ptr), position, owner, registry_);
    }

    rosa_assert(1 == 2, "Unknown 'class' name for object: {cname}");
    return std::shared_ptr<Object>(nullptr);
}

Shape* Map::getShape(const nlohmann::json& json) {
    spdlog::info("shape 1");
    if (json.find("shape") == json.end()) {
        spdlog::error("Error in parsing json: {}", json.dump());
        return nullptr;
    } 
    spdlog::info("shape 2");
    auto& shape_json = json.at("shape");
    if (shape_json.find("class") == shape_json.end()) {
        spdlog::error("Error in parsing json: {}", shape_json.dump());
        return nullptr;
    }

    Shape* shape;
        spdlog::info("shape 3");
    std::string type = shape_json.at("class").get<std::string>();
    if (type == "Shapeless") {
        shape = new Shapeless();
    } else if (type == "Cube") {
        shape = new Cube();
    } else if (type == "Cylinder") {
        shape = new Cylinder();
    } else {
        rosa_assert(1 == 2, "Unknown shape type");
    }
        spdlog::info("shape 4");

    shape->fromJson(shape_json);
    spdlog::info("shape 5");

    return shape;
}

Position Map::getPosition(const nlohmann::json& json) {
    Position p;
    p.fromJson(json);
    return p;
}

} // namespace rosa