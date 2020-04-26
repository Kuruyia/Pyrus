#ifndef PYRUS_VEC2D_H
#define PYRUS_VEC2D_H

#include <cstdint>

namespace Graphics {

struct Vec2D
{
    int16_t x;
    int16_t y;

    Graphics::Vec2D operator+(const Graphics::Vec2D& v) const
    {
        return {static_cast<int16_t>(x + v.x), static_cast<int16_t>(y + v.y)};
    }

    Graphics::Vec2D operator-(const Graphics::Vec2D& v) const
    {
        return {static_cast<int16_t>(x - v.x), static_cast<int16_t>(y - v.y)};
    }

    bool operator==(const Graphics::Vec2D& v) const
    {
        return x == v.x && y == v.y;
    }
}; // struct Vec2D

} // namespace Graphics

#endif //PYRUS_VEC2D_H
