#pragma once

#include "common/util.hpp"
#include "shape.hpp"
#include <memory>
#include <string>

namespace rosa {

class Box;

class Sphere : public Shape {
public:
    Sphere();

    Sphere(float radius);

    float getRadius() const { return radius_; }

    std::shared_ptr<Box> boundingBox(const Position& position) override;

    std::string getType() const override {return "Sphere";}
    
    bool isShapeless() const override { return false; }

    nlohmann::json toJson() const override {
        nlohmann::json dim = {{"radius", radius_}};
        nlohmann::json j = {{"type", "Sphere"}, {"params", dim}};
        return j;
    }

    void fromJson(const nlohmann::json& j) override {
        rosa_assert(j["type"].get<std::string>() == "Sphere",
                    "Problem in json conversion of Sphere");
        auto dim = j["params"];
        dim.at("radius").get_to(radius_);
    }

private:
    float radius_;
};

} // namespace rosa