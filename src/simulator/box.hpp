#pragma once

#include "cube.hpp"
#include "object.hpp"
#include "object_registry.hpp"
#include "position.hpp"

#include <memory>
#include <string>

namespace rosa {

class Box : public Object {
public:
    Box(ObjectId& oid,
        const std::string& name,
        std::unique_ptr<Cube>&& cube,
        Position& position,
        const std::shared_ptr<Object>& owner_object,
        std::shared_ptr<ObjectRegistry>& registry)
        : Object(oid, name, std::move(cube), position, owner_object, registry) {}

    virtual std::shared_ptr<Box> boundingBox() override { return nullptr; }

private:
    const Cube& getShape() const { return *(Cube*)shape_.get(); }
};

} // namespace rosa