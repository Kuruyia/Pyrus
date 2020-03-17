#include <utility>

#include "BaseWidget.h"

Widget::BaseWidget::BaseWidget(std::string id, const Vec2D_t position)
: m_parent(nullptr)
, m_id(std::move(id))
, m_dirty(true)
, m_clearLastPosition(false)
, m_position(position)
{

}

void Widget::BaseWidget::setPosition(Vec2D_t position)
{
    m_position = position;

    markDirty();
    m_clearLastPosition = true;
}

const Vec2D_t &Widget::BaseWidget::getPosition() const
{
    return m_position;
}

void Widget::BaseWidget::setParent(Widget::BaseContainer *parent)
{
    m_parent = parent;
}

const Widget::BaseContainer *Widget::BaseWidget::getParent() const
{
    return m_parent;
}

const std::string &Widget::BaseWidget::getId() const
{
    return m_id;
}

void Widget::BaseWidget::markDirty()
{
    m_dirty = true;
}
