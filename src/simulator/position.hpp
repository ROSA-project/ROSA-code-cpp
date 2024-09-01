#pragma once

#include "common/constants.hpp" // PI_CONST
#include "common/util.hpp"
#include "nlohmann/json.hpp"
#include <cmath>
#include <tuple>

namespace rosa {

struct Position {
    int x, y, z;
    double theta , i , j , k;

    Position()
        : Position(0, 0, 0, 0.0, 0.0 , 0.0 , 0.0) {}

    Position(int x, int y, int z, double theta, double i , double j , double k)
        : x(x)
        , y(y)
        , z(y)
        ,theta(theta)
        , i(i)
        , j(j)
        , k(j){};

    // static std::tuple<float, float, float>
    // polarToCartesian(float r, float phi, float theta) {
    //     float x = r * sin(theta * PI_CONST / 180) * cos(phi * PI_CONST / 180);
    //     float y = r * sin(theta * PI_CONST / 180) * sin(phi * PI_CONST / 180);
    //     float z = r * cos(theta * PI_CONST / 180);
    //     return std::make_tuple(x, y, z);
    // }

    // static std::tuple<int, int, int> cartesianToPolar(int x, int y, int z) {
    //     float r = sqrt(x * x + y * y + z * z);
    //     // TODO these arc functions need correct handling
    //     float theta = acos(z / r) * 180 / PI_CONST; // to degrees
    //     float phi = atan2(y, x) * 180 / PI_CONST;

    //     // TODO do we need to keep the qualities in position in native data types?
    //     return std::make_tuple(r, phi, theta);
    // }


    
    void  quaternion(){
        double q[4] {cos(theta) , sin(theta)*i, sin(theta)*j , sin(theta)*k }  ;
        // q must be written to json file for the object at the given delta_t
    }

    nlohmann::json toJson() const {
        nlohmann::json j = {{"x", x}, {"y", y}, {"z", z}};
        return j;
    }

    void fromJson(const nlohmann::json& j) {
        if (j.find("x") == j.end() || j.find("y") == j.end() || j.find("z") == j.end()) {
            rosa_assert(1 == 2, "Invalid arguments for position json");
        }
        j.at("x").get_to(x);
        j.at("y").get_to(y);
        j.at("z").get_to(z);
    }
};

} // namespace rosa