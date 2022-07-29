#pragma once

#include "nlohmann/json.hpp"
#include "position.hpp"
#include "shape.hpp"
#include "utils/util.hpp"
#include <memory>

namespace rosa {

class Box;

class Shapeless : public Shape {
public:
    Shapeless() {}

    std::shared_ptr<Box> boundingBox(const Position& position) override {
        return std::shared_ptr<Box>(nullptr);
    }

    bool isShapeless() const override { return true; }

    nlohmann::json toJson() const override {
        nlohmann::json j = {{"type", "Shapeless"}};
        return j;
    }

    void fromJson(const nlohmann::json& j) override {
        rosa_assert(j["type"].get<std::string>() == "Shapeless",
                    "Problem in json conversion of Shapeless");
    }
};

} // namespace rosa