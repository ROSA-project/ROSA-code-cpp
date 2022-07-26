#include "map.hpp"
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
            nlohmann::json parsed = json::parse(ifs);
            std::shared_ptr<ObjectRegistry> registry;
            std::unordered_map<ObjectId, std::shared_ptr<Object>> new_objects;
            getObjects(new_objects, parsed, None);
            registry->addObjects(new_objects);
            return registry;
    }
    else {
        // TODO: print sth on the logger
        throw std::invalid_argument("Error in opening file: " + filename);
    }


    try:
    with open(filename, "r") as f:
        parsed = json.load(f)
        obj_map = {}
        self.get_objects(obj_map, parsed, None)
        self.registry.add_objects(obj_map)
        return self.registry
except (OSError, IOError) as e:
    print("Error in opening file ", filename)
    raise e
}


} // namespace rosa