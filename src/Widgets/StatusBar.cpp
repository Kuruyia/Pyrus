#include "../Graphics/GfxUtils.h"
#include "BaseContainer.h"

#include "StatusBar.h"

Widget::StatusBar::StatusBar(const std::string &id, const Graphics::Vec2D position, const int16_t width,
                             const FONT_INFO *fontInfo, const std::string &primaryText, const std::string &secondaryText)
: BaseContainer(id, position, {0, 0, 0})
, m_size({width, 0})
, m_lastPosition(position)
, m_lastSize({width, 0})
, m_mainText(nullptr)
, m_secondaryText(nullptr)
{
    m_mainText = dynamic_cast<Text *>(&addChild(std::make_unique<Widget::Text>("statusPrimary", primaryText,
                                                                               fontInfo, Graphics::Vec2D{4, 0},
                                                                               Graphics::Color(255, 255, 255))));
    m_secondaryText = dynamic_cast<Text *>(&addChild(std::make_unique<Widget::Text>("statusSecondary", secondaryText, fontInfo,
                                                                                    Graphics::Vec2D{static_cast<int16_t>(width - 4), 0},
                                                                                    Graphics::Color(200, 200, 200))));

    m_size.y = (m_mainText->getHeight() > m_secondaryText->getHeight()) ? m_mainText->getHeight() : m_secondaryText->getHeight();
    m_size.y += 3;

    m_lastSize.y = m_size.y;

    m_secondaryText->setHorizontalAlignment(Widget::Text::HorizontalAlignment::Right);
}

void Widget::StatusBar::draw(Hardware::Screen::BaseScreen &target)
{
    // Dirty flag not set, we don't need to redraw the widget
    if (m_dirty == 0) return;

    // Geometry has changed, we need to clear the last occupied space
    if (isDirty(DirtyState::Global) || isDirty(DirtyState::Position) || isDirty(DirtyState::Size))
    {
        Graphics::Vec2D lastAbsolutePosition = getLastAbsolutePosition();
        if (m_loopVerticalPosition)
            lastAbsolutePosition.y %= target.getFramebufferSize().y;

        Graphics::GfxUtils::drawFilledRectangle(target, lastAbsolutePosition, m_lastSize,
                                                getParentBackgroundColor(), m_loopVerticalPosition);
    }

    // Draw children widgets
    m_mainText->draw(target);
    m_secondaryText->draw(target);

    Graphics::GfxUtils::drawFilledRectangle(target, {m_position.x, static_cast<int16_t>(m_position.y + m_size.y - 1)},
                                            {m_size.x, 1}, {255, 255, 0}, true);

    // Reset the dirty flag
    clearDirty();

    // Draw children
    for (auto &widget: m_children)
        widget->draw(target);
}

void Widget::StatusBar::setPosition(Graphics::Vec2D position)
{
    m_lastPosition = m_position;

    BaseContainer::setPosition(position);
}

Graphics::Vec2D Widget::StatusBar::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

void Widget::StatusBar::setWidth(int16_t width)
{
    m_lastSize = m_size;
    m_size.x = width;

    setDirty(DirtyState::Size, true);
    m_mainText->setDirty(DirtyState::Global, true);

    m_secondaryText->setPosition({static_cast<int16_t>(width - 4), 0});
}

int16_t Widget::StatusBar::getWidth() const
{
    return m_size.x;
}

int16_t Widget::StatusBar::getHeight() const
{
    return m_size.y;
}

Graphics::Vec2D Widget::StatusBar::getSize() const
{
    return m_size;
}

Widget::Text &Widget::StatusBar::getMainText()
{
    return *m_mainText;
}

Widget::Text &Widget::StatusBar::getSecondaryText()
{
    return *m_secondaryText;
}

Graphics::Vec2D Widget::StatusBar::getLastAbsolutePosition() const
{
    if (m_parent == nullptr)
        return m_lastPosition;

    return m_parent->getAbsolutePosition() + m_lastPosition;
}
