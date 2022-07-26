#pragma once

#include "ball.hpp"
#include "cube.hpp"
#include "cylinder.hpp"
#include "object.hpp"
#include "object_registery.hpp"
#include "position.hpp"
#include "vacuum_cleaner.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace rosa {

class Map {
public:
    Map::Map();
    
    /**
     * Loads the input json file and creates the objects.
     * 
     * @param map_filename The address of the json map file.
     * @return A new object registry populated with the objects specified in the map. 
     */
    std::shared_ptr<ObjectRegistry> parseMap(const std::string& map_filename);

    std::shared_ptr<Object> instantiateObject(obj_json, ObjectId new_id, const std::string& name, std::shared_ptr<Object>  owner);

    static std::shared_ptr<Shape> getShape(obj_json);

    static Position getPosition(obj_json);

    const std::unordered_map<ObjectId, std::shared_ptr<Object>>& getObjects(parsed, std::shared_ptr<Object> owner);

private:
    ObjectId nextAvailableId_;
    std::shared_ptr<ObjectRegistry> registry_;
};

} // namespace rosa