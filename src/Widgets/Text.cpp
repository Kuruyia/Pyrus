#include "Text.h"

#define INTERCHAR_SIZE 2

Widget::Text::Text(const std::string &text, const FONT_INFO *fontInfo, Vec2D_t position, Color565_t textColor,
                   Color565_t backgroundColor)
: m_dirty(true)
, m_clearLastPosition(false)
, m_text(text)
, m_fontInfo(fontInfo)
, m_position(position)
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
    if (!m_dirty) return;

    // Geometry has changed, we need to clear the last occupied space
    if (m_clearLastPosition)
    {
        target.drawRectangle(m_lastPosition, m_lastSize, m_backgroundColor);
        m_clearLastPosition = false;
    }

    // Store the position of this drawing
    Vec2D_t position = m_position;
    m_lastPosition = m_position;

    // Loop through all the characters and draw them
    for (const char c : m_text)
    {
        if (c != ' ')
        {
            uint16_t charWidth = target.drawChar(position, c, *m_fontInfo, m_textColor, m_backgroundColor);
            position.x += charWidth;
        }
        else
        {
            target.drawRectangle(position, {m_fontInfo->spacePixels, m_fontInfo->height}, m_backgroundColor);
            position.x += m_fontInfo->spacePixels;
        }

        target.drawRectangle(position, {INTERCHAR_SIZE, m_fontInfo->height}, m_backgroundColor);
        position.x += INTERCHAR_SIZE;
    }

    // Store the size of this drawing
    m_lastSize = {static_cast<uint16_t>(position.x - m_position.x), m_fontInfo->height};

    // Reset the dirty flag
    m_dirty = false;
}

void Widget::Text::setText(const std::string &text)
{
    m_text = text;

    m_dirty = true;
    m_clearLastPosition = true;
}

const std::string &Widget::Text::getText() const
{
    return m_text;
}

void Widget::Text::setFont(const FONT_INFO *fontInfo)
{
    m_fontInfo = fontInfo;

    m_dirty = true;
    m_clearLastPosition = true;
}

const FONT_INFO *Widget::Text::getFont() const
{
    return m_fontInfo;
}

void Widget::Text::setPosition(Vec2D_t position)
{
    m_position = position;

    m_dirty = true;
    m_clearLastPosition = true;
}

const Vec2D_t &Widget::Text::getPosition() const
{
    return m_position;
}

void Widget::Text::setTextColor(Color565_t textColor)
{
    m_textColor = textColor;

    m_dirty = true;
}

const Color565_t &Widget::Text::getTextColor() const
{
    return m_textColor;
}

void Widget::Text::setBackgroundColor(Color565_t backgroundColor)
{
    m_backgroundColor = backgroundColor;

    m_dirty = true;
}

const Color565_t &Widget::Text::getBackgroundColor() const
{
    return m_backgroundColor;
}
