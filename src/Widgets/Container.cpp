#include <algorithm>
#include "Container.h"

Widget::Container::Container(BaseContainer *parent, Vec2D_t position, Vec2D_t size, Color565_t color)
: m_parent(parent)
, m_dirty(true)
, m_clearLastPosition(false)
, m_position(position)
, m_size(size)
, m_lastPosition({0, 0})
, m_lastSize({0, 0})
, m_color(color)
{
    if (parent != nullptr)
        parent->addChild(this);
}

Widget::Container::~Container()
{
    if (m_parent != nullptr)
        m_parent->removeChild(this);
}

void Widget::Container::draw(Hardware::Screen::BaseScreen &target)
{
    // Dirty flag not set, we don't need to redraw the widget
    if (!m_dirty) return;

    // Geometry has changed, we need to clear the last occupied space
    if (m_clearLastPosition)
    {
        target.drawRectangle(getLastAbsolutePosition(), m_lastSize, getParentBackgroundColor());
        m_clearLastPosition = false;
    }

    // Render the container
    target.drawRectangle(m_position, m_size, m_color);

    // Store the geometry of this drawing
    m_lastPosition = m_position;
    m_lastSize = m_size;

    // Render the container children
    for (BaseWidget *widget: m_children)
        widget->draw(target);
}

void Widget::Container::setPosition(Vec2D_t position)
{
    m_position = position;

    markDirty();
    m_clearLastPosition = true;
}

const Vec2D_t &Widget::Container::getPosition() const
{
    return m_position;
}

Vec2D_t Widget::Container::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

const Widget::BaseContainer *Widget::Container::getParent() const
{
    return m_parent;
}

void Widget::Container::setSize(Vec2D_t size)
{
    m_size = size;

    markDirty();
    m_clearLastPosition = true;
}

Vec2D_t Widget::Container::getSize() const
{
    return m_size;
}

void Widget::Container::setBackgroundColor(Color565_t color)
{
    m_color = color;

    markDirty();
}

const Color565_t &Widget::Container::getBackgroundColor() const
{
    return m_color;
}

void Widget::Container::addChild(Widget::BaseWidget *child)
{
    m_children.push_back(child);
}

void Widget::Container::removeChild(Widget::BaseWidget *child)
{
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

void Widget::Container::markDirty()
{
    m_dirty = true;

    // Mark the children dirty
    for (BaseWidget *widget: m_children)
        widget->markDirty();
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