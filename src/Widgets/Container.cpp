#include <algorithm>
#include "Container.h"

Widget::Container::Container(const std::string &id, Vec2D_t position, Vec2D_t size, Color565_t color)
: m_parent(nullptr)
, m_id(id)
, m_dirty(true)
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
        target.drawRectangle(m_position, m_size, m_color);

        // Store the geometry of this drawing
        m_lastPosition = m_position;
        m_lastSize = m_size;

        // Reset the dirty flag
        m_dirty = false;
    }

    // Render the container children
    for (std::unique_ptr<BaseWidget> &widget: m_children)
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

void Widget::Container::setParent(Widget::BaseContainer *parent)
{
    m_parent = parent;
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

void Widget::Container::setBackgroundColor(Color565_t color)
{
    m_color = color;

    markDirty();
}

const Color565_t &Widget::Container::getBackgroundColor() const
{
    return m_color;
}

void Widget::Container::addChild(std::unique_ptr<BaseWidget> child)
{
    child->setParent(this);
    m_children.push_back(std::move(child));
}

bool Widget::Container::removeChild(const std::string &id)
{
    for (auto iter = m_children.begin(); iter != m_children.end(); ++iter)
    {
        if ((*iter)->getId() == id)
        {
            m_children.erase(iter);
            return true;
        }
    }

    return false;
}

std::unique_ptr<Widget::BaseWidget> &Widget::Container::findChildById(const std::string &id)
{
    for (auto &widget : m_children)
    {
        if (widget->getId() == id)
            return widget;
    }

    throw;
}

const std::vector<std::unique_ptr<Widget::BaseWidget>> &Widget::Container::getChildren()
{
    return m_children;
}

const std::string &Widget::Container::getId() const
{
    return m_id;
}

void Widget::Container::markDirty()
{
    m_dirty = true;

    // Mark the children dirty
    for (std::unique_ptr<BaseWidget> &widget: m_children)
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
