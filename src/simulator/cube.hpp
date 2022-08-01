#pragma once

#include "shape.hpp"
#include "common/util.hpp"
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
        nlohmann::json dim = {{"l", length_}, {"h", height_}, {"w", width_}};
        nlohmann::json j = {{"type", "Cube"}, {"params", dim}};
        return j;
    }

    void fromJson(const nlohmann::json& j) override {
        rosa_assert(j["type"].get<std::string>() == "Cube",
                    "Problem in json conversion of Cube");
        auto dim = j["params"];
        dim.at("l").get_to(length_);
        dim.at("h").get_to(height_);
        dim.at("w").get_to(width_);
    }

private:
    float length_;
    float height_;
    float width_;
};

} // namespace rosa