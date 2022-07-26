#pragma once

#include "utils/constants.hpp" // PI_CONST
#include <cmath>
#include <tuple>


namespace rosa {

struct Position {
    int x, y, z, phi, theta;

    Position()
        : Position(0, 0, 0, 0, 0) {}

    Position(int x, int y, int z, int phi, int theta)
        : x(x)
        , y(y)
        , z(y)
        , phi(y)
        , theta(y) {}

    
    static std::tuple<float, float, float> polarToCartesian(float r, float phi, float theta) {
        float x = r * sin(theta * PI_CONST / 180) * cos(phi * PI_CONST / 180);
        float y = r * sin(theta * PI_CONST / 180) * sin(phi * PI_CONST / 180);
        float z = r * cos(theta * PI_CONST / 180);
        return std::make_tuple(x, y, z);
    }

    static std::tuple<int, int, int> cartesianToPolar(int x, int y, int z) {
        float r = sqrt(x * x + y * y + z * z);
        // TODO these arc functions need correct handling
        float theta = acos(z / r) * 180 / PI_CONST; // to degrees
        float phi = atan2(y, x) * 180 / PI_CONST;

        // TODO do we need to keep the qualities in position in native data types?
        return std::make_tuple(r, phi, theta);
    }
};

} // namespace rosa