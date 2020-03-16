#ifndef PYRUS_CONTAINER_H
#define PYRUS_CONTAINER_H

#include "Hardware/Screen/BaseScreen.h"

namespace Widget
{

class Container {
public:
    Container(Vec2D_t position, Vec2D_t size, Color565_t color);

    void draw(Hardware::Screen::BaseScreen &target);

    void setPosition(Vec2D_t position);
    const Vec2D_t &getPosition() const;

    void setSize(Vec2D_t size);
    const Vec2D_t &getSize() const;

    void setColor(Color565_t color);
    const Color565_t &getColor() const;

private:
    bool m_dirty;
    bool m_clearLastPosition;

    Vec2D_t m_position;
    Vec2D_t m_size;

    Vec2D_t m_lastPosition;
    Vec2D_t m_lastSize;

    Color565_t m_color;
}; // class Container

} // namespace Widget

#endif //PYRUS_CONTAINER_H
