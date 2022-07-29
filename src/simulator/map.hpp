#pragma once

#include "object.hpp"
#include "object_registry.hpp"
#include "position.hpp"
#include "nlohmann/json.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace rosa {

class Map {
public:
    Map();
    
    /**
     * Loads the input json file and creates the objects.
     * 
     * @param map_filename The address of the json map file.
     * @return A new object registry populated with the objects specified in the map. 
     */
    std::shared_ptr<ObjectRegistry> parseMap(const std::string& map_filename);

private:
    ObjectId nextAvailableId_;
    std::shared_ptr<ObjectRegistry> registry_;

    
    // --- METHODS ---

    /**
     * stores a dictionary of objects at this current level (and not deeper)
     */
    const Object::ObjectMap& getObjects(std::shared_ptr<ObjectRegistry> registry, Object::ObjectMap& obj_map, const nlohmann::json &json, std::shared_ptr<Object> owner);

    std::shared_ptr<Object> instantiateObject(std::shared_ptr<ObjectRegistry> registry, const nlohmann::json &json, ObjectId new_id, const std::string& name, std::shared_ptr<Object>  owner);

    static Shape* getShape(const nlohmann::json &json);

    static Position getPosition(const nlohmann::json &json);
};

} // namespace rosa