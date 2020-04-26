#include <algorithm>
#include "Container.h"

Widget::Container::Container(const std::string &id, Graphics::Vec2D position, Graphics::Vec2D size, const Graphics::Color &backgroundColor)
: BaseContainer(id, position, backgroundColor)
, m_size(size)
, m_lastPosition({0, 0})
, m_lastSize({0, 0})
{

}

void Widget::Container::draw(Hardware::Screen::BaseScreen &target)
{
    // Check if we need to redraw the widget itself
    if (m_dirty.any())
    {
        // Geometry has changed, we need to clear the last occupied space
        if (isDirty(DirtyState::Position) || isDirty(DirtyState::Size))
        {
            Graphics::Vec2D lastAbsolutePosition = getLastAbsolutePosition();
            if (m_loopVerticalPosition)
                lastAbsolutePosition.y %= target.getFramebufferSize().y;

            target.drawRectangle(lastAbsolutePosition, m_lastSize, getParentBackgroundColor(), m_loopVerticalPosition);
            setDirty(DirtyState::Position, false);
        }

        // Loop the vertical axis if enabled
        Graphics::Vec2D position = getAbsolutePosition();
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

Graphics::Vec2D Widget::Container::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

void Widget::Container::setSize(Graphics::Vec2D size)
{
    m_size = size;

    setDirty(DirtyState::Size, true);
}

int16_t Widget::Container::getWidth() const
{
    return m_size.x;
}

int16_t Widget::Container::getHeight() const
{
    return m_size.y;
}

Graphics::Vec2D Widget::Container::getSize() const
{
    return m_size;
}

Graphics::Vec2D Widget::Container::getLastAbsolutePosition() const
{
    if (m_parent == nullptr)
        return m_lastPosition;

    return m_parent->getAbsolutePosition() + m_lastPosition;
}

Graphics::Color Widget::Container::getParentBackgroundColor() const
{
    if (m_parent == nullptr)
        return {0, 0, 0};

    return m_parent->getBackgroundColor();
}
