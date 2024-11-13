#pragma once

#include "nlohmann/json.hpp"

namespace rosa {

struct Velocity {
    double v_x , v_y , v_z , omega;

    Velocity()
        : Velocity(1.0, 0.0, 0.0 , 1.0) {}

    Velocity(double v_x, double v_y, double v_z, double omega)
        : v_x(v_x)
        , v_y(v_y)
        , v_z(v_z)
        , omega(omega)
        {}

        void fromJson(const nlohmann::json& j) {
        if (j.find("v_x") == j.end() || j.find("v_y") == j.end() || j.find("v_z") == j.end() || j.find("omega") == j.end()) {
            rosa_assert(1 == 2, "Invalid arguments for velocity json");
        }
        j.at("v_x").get_to(v_x);
        j.at("v_y").get_to(v_y);
        j.at("v_z").get_to(v_z);
        j.at("omega").get_to(omega);
      
    }




};

// struct State {
//     Position position {};
//     Orientation Orientation {};
//     Velocity velocity {};
//     double omega {};
// };

}




