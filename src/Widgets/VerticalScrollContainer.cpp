#include <libraries/delay/nrf_delay.h>

#include "../Graphics/GfxUtils.h"
#include "VerticalScrollContainer.h"

Widget::VerticalScrollContainer::VerticalScrollContainer(const std::string &id, Graphics::Vec2D position, Graphics::Vec2D size,
                                                         const Graphics::Color &backgroundColor)
: BaseContainer(id, position, backgroundColor)
, m_size(size)
, m_lastPosition({0, 0})
, m_lastSize({0, 0})
, m_verticalScrollOffsetChanged(false)
, m_verticalScrollOffset(0)
{
    // Set the new child handler
    m_newChildHandler = [](BaseWidget &widget) {
        widget.setLoopVerticalPosition(true);
    };
}

void Widget::VerticalScrollContainer::draw(Hardware::Screen::BaseScreen &target)
{
    // Check if we need to redraw the widget itself
    if (m_dirty.any())
    {
        // Geometry has changed, we need to clear the last occupied space
        if (isDirty(DirtyState::Global) || isDirty(DirtyState::Position) || isDirty(DirtyState::Size))
        {
            Graphics::GfxUtils::drawFilledRectangle(target, getLastAbsolutePosition(), m_lastSize,
                                                    getParentBackgroundColor(), m_loopVerticalPosition);
        }

        // Render the container
        Graphics::GfxUtils::drawFilledRectangle(target, m_position, m_size, m_backgroundColor, m_loopVerticalPosition);

        // Store the geometry of this drawing
        m_lastPosition = m_position;
        m_lastSize = m_size;

        // Reset the dirty flag
        clearDirty();
    }

    // Send the vertical offset to the target
    if (m_verticalScrollOffsetChanged)
        target.setVerticalScrollOffset(m_verticalScrollOffset % target.getFramebufferSize().y);

    // Render the container children
    const uint16_t scrollEnd = m_verticalScrollOffset + target.getScreenSize().y;
    for (auto &widget: m_children)
    {
        const Graphics::Vec2D widgetAbsolutePosition = widget->getAbsolutePosition();
        if (widgetAbsolutePosition.y + widget->getHeight() >= m_verticalScrollOffset &&
                widgetAbsolutePosition.y < scrollEnd)
        {
            widget->draw(target);
        }
        else
        {
            widget->setDirty(DirtyState::Global, true);
        }
    }
}

Graphics::Vec2D Widget::VerticalScrollContainer::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

void Widget::VerticalScrollContainer::setSize(Graphics::Vec2D size)
{
    m_size = size;

    setDirtyWithChildren(DirtyState::Size, true);
}

int16_t Widget::VerticalScrollContainer::getWidth() const
{
    return m_size.x;
}

int16_t Widget::VerticalScrollContainer::getHeight() const
{
    return m_size.y;
}

Graphics::Vec2D Widget::VerticalScrollContainer::getSize() const
{
    return m_size;
}

Graphics::Vec2D Widget::VerticalScrollContainer::getLastAbsolutePosition() const
{
    if (m_parent == nullptr)
        return m_lastPosition;

    return m_parent->getAbsolutePosition() + m_lastPosition;
}

Graphics::Color Widget::VerticalScrollContainer::getParentBackgroundColor() const
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
