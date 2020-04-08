#include "BaseContainer.h"

#include "StatusBar.h"

Widget::StatusBar::StatusBar(const std::string &id, const Vec2D_t position, const Vec2D_t size, const FONT_INFO *fontInfo)
: BaseWidget(id, position)
, m_size(size)
, m_mainText("statusPrimary", "Primary", fontInfo, position, {31, 63, 31})
, m_secondaryText("statusSecondary", "Secondary", fontInfo, position, {25, 51, 25})
{
    m_maxHeight = (m_mainText.getHeight() > m_secondaryText.getHeight()) ? m_mainText.getHeight() : m_secondaryText.getHeight();
}

void Widget::StatusBar::draw(Hardware::Screen::BaseScreen &target)
{
    m_mainText.draw(target);
    m_secondaryText.draw(target);

    target.drawRectangle({m_position.x, static_cast<uint16_t>(m_position.y + m_maxHeight)}, {m_size.x, 1}, {31, 63, 0}, true);
}

Vec2D_t Widget::StatusBar::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

uint16_t Widget::StatusBar::getWidth() const
{
    return m_size.y;
}

uint16_t Widget::StatusBar::getHeight() const
{
    return m_maxHeight;
}

Vec2D_t Widget::StatusBar::getSize() const
{
    return m_size;
}
