#pragma once

#include "object.hpp"
#include <atomic>
#include <memory>
#include <unordered_map>

namespace rosa {

class Object;

class ObjectRegistry {
public:
    ObjectRegistry()
    : nextId_(0){}

    // Returns the dictionary of all current objects with ObjectIDs as keys and Objects as values.
    std::unordered_map<ObjectId, std::shared_ptr<Object>>& getObjects() {
        return objects_;
    }

    /**
     * @brief Adds new objects.
     */
    void addObjects(const std::unordered_map<ObjectId, std::shared_ptr<Object>>& new_objects) {
        objects_.insert(new_objects.begin(), new_objects.end());
    }

    /**
     * @brief Generates and returns the next available id.
     */
    ObjectId getNextAvailableId() {
        return ++nextId_;
    }

private:
    std::unordered_map<ObjectId, std::shared_ptr<Object>> objects_;
    std::atomic<ObjectId> nextId_;
};

} // namespace rosa
