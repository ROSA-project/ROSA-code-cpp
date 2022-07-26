#pragma once

#include "common/util.hpp"
#include "shape.hpp"
#include <memory>

namespace rosa {

class Box;

class Cube : public Shape {
public:
    Cube();

    Cube(float length, float height, float width);

    float getLength() const { return length_; }

    float getHeight() const { return height_; }

    float getWidth() const { return width_; }

    std::shared_ptr<Box> boundingBox(const Position& position) override;

    bool isShapeless() const override { return false; }

    nlohmann::json toJson() const override {
        nlohmann::json dim = {
            {"length", length_}, {"height", height_}, {"width", width_}};
        nlohmann::json j = {{"type", "Cube"}, {"params", dim}};
        return j;
    }

    void fromJson(const nlohmann::json& j) override {
        rosa_assert(j["type"].get<std::string>() == "Cube",
                    "Problem in json conversion of Cube");
        auto dim = j["params"];
        dim.at("length").get_to(length_);
        dim.at("height").get_to(height_);
        dim.at("width").get_to(width_);
    }

private:
    float length_;
    float height_;
    float width_;
};

} // namespace rosa