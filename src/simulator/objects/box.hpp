#pragma once

#include "object.hpp"
#include "object_registry.hpp"
#include "position.hpp"
#include "shapes/cube.hpp"

#include <memory>
#include <string>

namespace rosa {

class Box: public Object {
public:
    Box(ObjectId& oid, const std::string &name, std::unique_ptr<Cube> &&cube, Position &position,
                 const std::shared_ptr<Object>& owner_object, std::shared_ptr<ObjectRegistry> &registry)
                 : Object(oid, name, std::move(cube), position, owner_object, registry)
                 {}
    virtual std::shared_ptr<Box> bounding_box() {
        return nullptr;
    }

};

} // namespace rosa