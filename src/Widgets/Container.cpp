#include "Container.h"

Widget::Container::Container(Vec2D_t position, Vec2D_t size, Color565_t color)
: m_dirty(true)
, m_clearLastPosition(false)
, m_position(position)
, m_size(size)
, m_lastPosition({0, 0})
, m_lastSize({0, 0})
, m_color(color)
{

}

void Widget::Container::draw(Hardware::Screen::BaseScreen &target)
{
    // Dirty flag not set, we don't need to redraw the widget
    if (!m_dirty) return;

    // Geometry has changed, we need to clear the last occupied space
    if (m_clearLastPosition)
    {
        target.drawRectangle(m_lastPosition, m_lastSize, m_color);
        m_clearLastPosition = false;
    }

    // Render the container
    target.drawRectangle(m_position, m_size, m_color);

    // Store the geometry of this drawing
    m_lastPosition = m_position;
    m_lastSize = m_size;
}

void Widget::Container::setPosition(Vec2D_t position)
{
    m_position = position;

    m_dirty = true;
    m_clearLastPosition = true;
}

const Vec2D_t &Widget::Container::getPosition() const
{
    return m_position;
}

void Widget::Container::setSize(Vec2D_t size)
{
    m_size = size;

    m_dirty = true;
    m_clearLastPosition = true;
}

const Vec2D_t &Widget::Container::getSize() const
{
    return m_size;
}

void Widget::Container::setColor(Color565_t color)
{
    m_color = color;

    m_dirty = true;
}

const Color565_t &Widget::Container::getColor() const
{
    return m_color;
}
