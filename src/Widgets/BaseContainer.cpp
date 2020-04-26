#include <utility>

#include "BaseContainer.h"

Widget::BaseContainer::BaseContainer(std::string id, Graphics::Vec2D position, const Graphics::Color &backgroundColor)
: BaseWidget(std::move(id), position)
, m_backgroundColor(backgroundColor)
{

}

void Widget::BaseContainer::setPosition(Graphics::Vec2D position)
{
    BaseWidget::setPosition(position);

    setDirtyWithChildren(DirtyState::Position, true);
}

Widget::BaseWidget &Widget::BaseContainer::addChild(std::unique_ptr<BaseWidget> child)
{
    child->setParent(this);
    if (m_newChildHandler)
        m_newChildHandler(*(child.get()));

    m_children.push_back(std::move(child));
    return *(m_children.back().get());
}

bool Widget::BaseContainer::removeChild(const std::string &id)
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

std::unique_ptr<Widget::BaseWidget> &Widget::BaseContainer::findChildById(const std::string &id)
{
    for (auto &widget : m_children)
    {
        if (widget->getId() == id)
            return widget;
    }

    throw;
}

const std::vector<std::unique_ptr<Widget::BaseWidget>> &Widget::BaseContainer::getChildren()
{
    return m_children;
}

void Widget::BaseContainer::setBackgroundColor(const Graphics::Color &color)
{
    m_backgroundColor = color;

    setDirtyWithChildren(DirtyState::Color, true);
}

const Graphics::Color &Widget::BaseContainer::getBackgroundColor() const
{
    return m_backgroundColor;
}

void Widget::BaseContainer::setDirtyWithChildren(DirtyState state, bool dirty)
{
    BaseWidget::setDirty(state, dirty);

    // Mark the children dirty
    for (auto &widget: m_children)
        widget->setDirty(state, dirty);
}
