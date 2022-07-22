#pragma once

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
};

} // namespace rosa