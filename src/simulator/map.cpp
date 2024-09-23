#include "map.hpp"

#include "ball.hpp"
#include "box.hpp"
#include "common/logger.hpp"
#include "common/util.hpp"
#include "cube.hpp"
#include "cylinder.hpp"
#include "sphere.hpp"
#include "shapeless.hpp"
#include "vacuum_cleaner.hpp"
#include "velocity.hpp"

#include <fstream>
#include <stdexcept>


namespace rosa {

bool json_has(const nlohmann::json& json, const std::string& elem) {
    return json.find(elem) != json.end();
}

Map::Map() {}

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
    Object::ObjectMap this_level_objects;
    for (auto& [oname, obj_json]: json.items()) {
        auto obj = instantiateObject(registry, obj_json, oname, owner);
        this_level_objects[obj->getObjectId()] = obj;

        if (json_has(obj_json, "subobjects")) {
            rosa_assert(obj_json["class"].get<std::string>() == "CompoundPhysical",
                        "Only CompoundPhysical objects can have subobjects");
            auto dependents = getObjects(registry, obj_map, obj_json["subobjects"], obj);

            // Add all dependent objects
            for (auto& p: dependents) {
                obj->addDependentObject(p.second);
            }
        }
        obj_map[obj->getObjectId()] = obj;
    }

    return this_level_objects;
}

std::shared_ptr<Object> Map::instantiateObject(std::shared_ptr<ObjectRegistry> registry,
                                               const nlohmann::json& json,
                                               const std::string& name,
                                               std::shared_ptr<Object> owner) {
    auto new_id = registry->getNextAvailableId();
    if (json_has(json, "tester-id")) {
        registry->addTesterID(new_id, json["tester-id"]);
    }

    Shape* shape = nullptr;
    shape = getShape(json);

    if (!json_has(json, "position")) {
        LOG_ERROR("Error in parsing json: {}", json.dump());
        return nullptr;
    }

    auto position = getPosition(json["position"]);
    auto cname = json["class"].get<std::string>();
    if (cname == "Box") {
        auto u_ptr = std::unique_ptr<Cube>((Cube*)shape);
        return std::make_shared<Box>(
            new_id, name, std::move(u_ptr), position, owner, registry_);
    } else if (cname == "RigidPointBall") {
        // TODO hardcoding acceleration and velocity not to change
        // Erfan's code w/o discussion
        auto u_ptr = std::unique_ptr<Sphere>((Sphere*)shape);
        Velocity v(1.0, 0 , 0.0 , 0.0);
        return std::make_shared<RigidPointBall>(
            new_id, name, std::move(u_ptr), position, owner, registry_, v , 2);
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
    if (!json_has(json, "shape")) {
        // Must be compound object
        MUST_TRUE(json_has(json, "class") && json["class"] == "CompoundPhysical");
        return new Shapeless();
    }

    auto& json_shape = json["shape"];

    if (!json_has(json_shape, "type")) {
        LOG_ERROR("Error in parsing json, 'shape' has no 'type': {}", json_shape.dump());
        return nullptr;
    }

    Shape* shape;
    std::string type = json_shape.at("type").get<std::string>();
    if (type == "Shapeless") {
        shape = new Shapeless();
    } else if (type == "Cube") {
        shape = new Cube();
    } else if (type == "Cylinder") {
        shape = new Cylinder();
    } else if (type == "Sphere") {
        shape = new Sphere();
    } else {
        rosa_assert(1 == 2, "Unknown shape type");
    }

    shape->fromJson(json_shape);

    return shape;
}

Position Map::getPosition(const nlohmann::json& json) {
    Position p;
    p.fromJson(json);
    return p;
}

} // namespace rosa