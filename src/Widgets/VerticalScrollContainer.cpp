#include <libraries/delay/nrf_delay.h>
#include "VerticalScrollContainer.h"

Widget::VerticalScrollContainer::VerticalScrollContainer(const std::string &id, Vec2D_t position, Vec2D_t size, Color565_t backgroundColor)
: BaseContainer(id, position, backgroundColor)
, m_size(size)
, m_lastPosition({0, 0})
, m_lastSize({0, 0})
, m_verticalScrollOffsetChanged(false)
, m_verticalScrollOffset(0)
{

}

void Widget::VerticalScrollContainer::draw(Hardware::Screen::BaseScreen &target)
{
    // Check if we need to redraw the widget itself
    if (m_dirty)
    {
        // Geometry has changed, we need to clear the last occupied space
        if (m_clearLastPosition)
        {
            target.drawRectangle(getLastAbsolutePosition(), m_lastSize, getParentBackgroundColor());
            m_clearLastPosition = false;
        }

        // Render the container
        target.drawRectangle(m_position, m_size, m_backgroundColor);

        // Store the geometry of this drawing
        m_lastPosition = m_position;
        m_lastSize = m_size;

        // Reset the dirty flag
        m_dirty = false;
    }

    // Send the vertical offset to the target
    if (m_verticalScrollOffsetChanged)
        target.setVerticalScrollOffset(m_verticalScrollOffset % target.getFramebufferSize().y);

    // Render the container children
    const uint16_t scrollEnd = m_verticalScrollOffset + target.getScreenSize().y;
    for (auto &widget: m_children)
    {
        const Vec2D_t widgetAbsolutePosition = widget.second->getAbsolutePosition();
        if (widgetAbsolutePosition.y + widget.second->getHeight() >= m_verticalScrollOffset &&
                widgetAbsolutePosition.y < scrollEnd)
        {
            widget.second->draw(target);
        }
        else
        {
            widget.second->markDirty();
        }
    }
}

Vec2D_t Widget::VerticalScrollContainer::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

void Widget::VerticalScrollContainer::addChild(std::unique_ptr<BaseWidget> child)
{
    child->setLoopVerticalPosition(true);
    BaseContainer::addChild(std::move(child));
}

void Widget::VerticalScrollContainer::setSize(Vec2D_t size)
{
    m_size = size;

    markDirtyWithChildren();
    m_clearLastPosition = true;
}

uint16_t Widget::VerticalScrollContainer::getWidth() const
{
    return m_size.x;
}

uint16_t Widget::VerticalScrollContainer::getHeight() const
{
    return m_size.y;
}

Vec2D_t Widget::VerticalScrollContainer::getSize() const
{
    return m_size;
}

Vec2D_t Widget::VerticalScrollContainer::getLastAbsolutePosition() const
{
    if (m_parent == nullptr)
        return m_lastPosition;

    return m_parent->getAbsolutePosition() + m_lastPosition;
}

Color565_t Widget::VerticalScrollContainer::getParentBackgroundColor() const
{
    if (m_parent == nullptr)
        return {0, 0, 0};

    return m_parent->getBackgroundColor();
}

void Widget::VerticalScrollContainer::setVerticalScrollOffset(uint16_t offset)
{
    m_verticalScrollOffset = offset;
    m_verticalScrollOffsetChanged = true;
}

uint16_t Widget::VerticalScrollContainer::getVerticalScrollOffset() const
{
    return m_verticalScrollOffset;
}
