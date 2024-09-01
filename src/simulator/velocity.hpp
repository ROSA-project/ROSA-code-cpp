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




};

// struct State {
//     Position position {};
//     Orientation Orientation {};
//     Velocity velocity {};
//     double omega {};
// };

}




