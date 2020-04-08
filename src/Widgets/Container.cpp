#include <algorithm>
#include "Container.h"

Widget::Container::Container(const std::string &id, Vec2D_t position, Vec2D_t size, Color565_t backgroundColor)
: BaseContainer(id, position, backgroundColor)
, m_size(size)
, m_lastPosition({0, 0})
, m_lastSize({0, 0})
{

}

void Widget::Container::draw(Hardware::Screen::BaseScreen &target)
{
    // Check if we need to redraw the widget itself
    if (m_dirty)
    {
        // Geometry has changed, we need to clear the last occupied space
        if (isDirty(DirtyState::Position) || isDirty(DirtyState::Size))
        {
            Vec2D_t lastAbsolutePosition = getLastAbsolutePosition();
            if (m_loopVerticalPosition)
                lastAbsolutePosition.y %= target.getFramebufferSize().y;

            target.drawRectangle(lastAbsolutePosition, m_lastSize, getParentBackgroundColor(), m_loopVerticalPosition);
            setDirty(DirtyState::Position, false);
        }

        // Loop the vertical axis if enabled
        Vec2D_t position = getAbsolutePosition();
        if (m_loopVerticalPosition)
            position.y %= target.getFramebufferSize().y;

        // Render the container
        target.drawRectangle(position, m_size, m_backgroundColor, m_loopVerticalPosition);

        // Store the geometry of this drawing
        m_lastPosition = m_position;
        m_lastSize = m_size;

        // Reset the dirty flag
        clearDirty();
    }

    // Render the container children
    for (auto &widget: m_children)
        widget->draw(target);
}

Vec2D_t Widget::Container::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

void Widget::Container::setSize(Vec2D_t size)
{
    m_size = size;

    setDirty(DirtyState::Size, true);
}

uint16_t Widget::Container::getWidth() const
{
    return m_size.x;
}

uint16_t Widget::Container::getHeight() const
{
    return m_size.y;
}

Vec2D_t Widget::Container::getSize() const
{
    return m_size;
}

Vec2D_t Widget::Container::getLastAbsolutePosition() const
{
    if (m_parent == nullptr)
        return m_lastPosition;

    return m_parent->getAbsolutePosition() + m_lastPosition;
}

Color565_t Widget::Container::getParentBackgroundColor() const
{
    if (m_parent == nullptr)
        return {0, 0, 0};

    return m_parent->getBackgroundColor();
}
