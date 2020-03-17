#include <utility>

#include "BaseContainer.h"

Widget::BaseContainer::BaseContainer(std::string id, Vec2D_t position, Color565_t backgroundColor)
: BaseWidget(std::move(id), position)
, m_backgroundColor(backgroundColor)
{

}

void Widget::BaseContainer::addChild(std::unique_ptr<BaseWidget> child)
{
    child->setParent(this);
    m_children.push_back(std::move(child));
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

void Widget::BaseContainer::setBackgroundColor(Color565_t color)
{
    m_backgroundColor = color;

    markDirty();
}

const Color565_t &Widget::BaseContainer::getBackgroundColor() const
{
    return m_backgroundColor;
}