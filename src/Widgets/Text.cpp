#include "Text.h"
#include "Container.h"

#define INTERCHAR_SIZE 2

Widget::Text::Text(const std::string &id, const std::string &text, const FONT_INFO *fontInfo, Vec2D_t position,
                   Color565_t textColor, Color565_t backgroundColor)
: BaseWidget(id, position)
, m_text(text)
, m_fontInfo(fontInfo)
, m_lastPosition({0, 0})
, m_lastSize({0, 0})
, m_textColor(textColor)
, m_backgroundColor(backgroundColor)
{
    m_fontInfo = fontInfo;
}

void Widget::Text::draw(Hardware::Screen::BaseScreen &target)
{
    // Dirty flag not set, we don't need to redraw the widget
    if (m_dirty == 0) return;

    // Geometry has changed, we need to clear the last occupied space
    if (isDirty(DirtyState::Global) || isDirty(DirtyState::Position) || isDirty(DirtyState::Size))
    {
        Vec2D_t lastAbsolutePosition = getLastAbsolutePosition();
        if (m_loopVerticalPosition)
            lastAbsolutePosition.y %= target.getFramebufferSize().y;

        target.drawRectangle(lastAbsolutePosition, m_lastSize, getParentBackgroundColor(), m_loopVerticalPosition);
    }

    // Store the position of this drawing
    Vec2D_t position = getAbsolutePosition();
    m_lastPosition = m_position;

    // Loop the vertical axis if enabled
    if (m_loopVerticalPosition)
        position.y %= target.getFramebufferSize().y;

    // Draw the background
    target.drawRectangle(position, getSize(), m_backgroundColor, m_loopVerticalPosition);

    // Loop through all the characters and draw them
    for (const char c : m_text)
    {
        if (c != ' ')
        {
            uint16_t charWidth = target.drawChar(position, c, *m_fontInfo, m_textColor, m_backgroundColor, m_loopVerticalPosition);
            position.x += charWidth;
        }
        else
        {
            position.x += m_fontInfo->spacePixels;
        }

        position.x += INTERCHAR_SIZE;
    }

    // Store the size of this drawing
    m_lastSize = {static_cast<uint16_t>(position.x - m_position.x), m_fontInfo->height};

    // Reset the dirty flag
    clearDirty();
}

void Widget::Text::setText(const std::string &text)
{
    m_text = text;

    setDirty(DirtyState::Size, true);
}

const std::string &Widget::Text::getText() const
{
    return m_text;
}

void Widget::Text::setFont(const FONT_INFO *fontInfo)
{
    m_fontInfo = fontInfo;

    setDirty(DirtyState::Size, true);
}

const FONT_INFO *Widget::Text::getFont() const
{
    return m_fontInfo;
}

Vec2D_t Widget::Text::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

uint16_t Widget::Text::getWidth() const
{
    // Prepare the width
    uint16_t width = 0;

    // Loop through all the characters and count their width
    for (const char c : m_text)
    {
        if (c != ' ')
        {
            const size_t descriptorOffset = c - m_fontInfo->startChar;
            const FONT_CHAR_INFO descriptor = m_fontInfo->charInfo[descriptorOffset];

            width += descriptor.widthBits;
        }
        else
        {
            width += m_fontInfo->spacePixels;
        }

        width += INTERCHAR_SIZE;
    }

    // Return the width
    return width;
}

uint16_t Widget::Text::getHeight() const
{
    return m_fontInfo->height;
}

Vec2D_t Widget::Text::getSize() const
{
    return {getWidth(), getHeight()};
}

void Widget::Text::setTextColor(Color565_t textColor)
{
    m_textColor = textColor;

    setDirty(DirtyState::Color, true);
}

const Color565_t &Widget::Text::getTextColor() const
{
    return m_textColor;
}

void Widget::Text::setBackgroundColor(Color565_t backgroundColor)
{
    m_backgroundColor = backgroundColor;

    setDirty(DirtyState::Color, true);
}

const Color565_t &Widget::Text::getBackgroundColor() const
{
    return m_backgroundColor;
}

Vec2D_t Widget::Text::getLastAbsolutePosition() const
{
    if (m_parent == nullptr)
        return m_lastPosition;

    return m_parent->getAbsolutePosition() + m_lastPosition;
}

Color565_t Widget::Text::getParentBackgroundColor() const
{
    if (m_parent == nullptr)
        return {0, 0, 0};

    return m_parent->getBackgroundColor();
}
