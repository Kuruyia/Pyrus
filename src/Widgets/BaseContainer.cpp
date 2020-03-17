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
    m_children[child->getId()] = std::move(child);
}

void Widget::BaseContainer::removeChild(const std::string &id)
{
    m_children.erase(id);
}

std::unique_ptr<Widget::BaseWidget> &Widget::BaseContainer::findChildById(const std::string &id)
{
    return m_children[id];
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