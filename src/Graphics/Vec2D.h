#ifndef PYRUS_VEC2D_H
#define PYRUS_VEC2D_H

#include <cstdint>

struct Vec2D_t
{
    uint16_t x;
    uint16_t y;

    Vec2D_t operator+(const Vec2D_t& v) const
    {
        return {static_cast<uint16_t>(x + v.x), static_cast<uint16_t>(y + v.y)};
    }

    Vec2D_t operator-(const Vec2D_t& v) const
    {
        return {static_cast<uint16_t>(x - v.x), static_cast<uint16_t>(y - v.y)};
    }

    bool operator==(const Vec2D_t& v) const
    {
        return x == v.x && y == v.y;
    }
};

#endif //PYRUS_VEC2D_H
