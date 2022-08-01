#pragma once

#include "shape.hpp"
#include "common/util.hpp"
#include <memory>

namespace rosa {

class Box;

class Cylinder : public Shape {
public:
    Cylinder();

    Cylinder(float radius, float height);

    float getRadius() const { return radius_; }

    float getHeight() const { return height_; }

    std::shared_ptr<Box> boundingBox(const Position& position) override;

    bool isShapeless() const override { return false; }

    nlohmann::json toJson() const override {
        nlohmann::json dim = {{"r", radius_}, {"h", height_}};
        nlohmann::json j = {{"type", "Cylinder"}, {"params", dim}};
        return j;
    }

    void fromJson(const nlohmann::json& j) override {
        rosa_assert(j["type"].get<std::string>() == "Cylinder",
                    "Problem in json conversion of Cylinder");
        auto dim = j["params"];
        dim.at("r").get_to(radius_);
        dim.at("h").get_to(height_);
    }

private:
    float radius_;
    float height_;
};

} // namespace rosa