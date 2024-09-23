#pragma once

#include "object.hpp"
#include <atomic>
#include <memory>
#include <unordered_map>
#include <cassert>

namespace rosa {

class Object;

class ObjectRegistry {
public:
    ObjectRegistry()
        : nextId_(0) {}

    /**
     * Returns the dictionary of all current objects with ObjectIDs as keys and Objects as
     * values.
     */
    std::unordered_map<ObjectId, std::shared_ptr<Object>>& getObjects() {
        return objects_;
    }

    /**
     * Adds new objects.
     */
    void
    addObjects(const std::unordered_map<ObjectId, std::shared_ptr<Object>>& new_objects) {
        objects_.insert(new_objects.begin(), new_objects.end());
    }

    /**
     * Generates and returns the next available id.
     */
    ObjectId getNextAvailableId() { return ++nextId_; }
    
    /**
     * Associating an ObjectID with a tester-ID. Tester IDs are directly from the map and does not affect
     * the simulator itself. 
     * TODO neither existence of the object ID nor unique-ness of the tester ID
     * is verified.
     */
    void addTesterID(ObjectId objID, int testerID) { testerIDs_.insert(std::make_pair(testerID, objID)); }

    ObjectId getTesterID(int testerID) {
        assert(testerIDs_.find(testerID) != testerIDs_.end() && "tester ID does not exist. See the map.");
        return testerIDs_[testerID];
    }

private:
    std::unordered_map<ObjectId, std::shared_ptr<Object>> objects_;
    std::unordered_map<ObjectId, int> testerIDs_;
    std::atomic<ObjectId> nextId_;
};

} // namespace rosa
