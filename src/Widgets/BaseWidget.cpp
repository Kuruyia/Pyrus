#include <utility>

#include "BaseWidget.h"

Widget::BaseWidget::BaseWidget(std::string id, const Vec2D_t position)
: m_parent(nullptr)
, m_id(std::move(id))
, m_dirty(1 << DirtyState::Global)
, m_position(position)
, m_loopVerticalPosition(false)
{

}

void Widget::BaseWidget::setPosition(Vec2D_t position)
{
    m_position = position;

    setDirty(DirtyState::Position, true);
}

const Vec2D_t &Widget::BaseWidget::getPosition() const
{
    return m_position;
}

void Widget::BaseWidget::setLoopVerticalPosition(bool loopVerticalPosition)
{
    m_loopVerticalPosition = loopVerticalPosition;
}

bool Widget::BaseWidget::isLoopingVerticalPosition() const
{
    return m_loopVerticalPosition;
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

void Widget::BaseWidget::setDirty(DirtyState state, bool dirty)
{
    if (dirty)
        m_dirty |= 1 << state;
    else
        m_dirty &= ~(1 << state);
}

void Widget::BaseWidget::clearDirty()
{
    m_dirty = 0;
}

bool Widget::BaseWidget::isDirty(Widget::BaseWidget::DirtyState state)
{
    return m_dirty & (1 << state);
}