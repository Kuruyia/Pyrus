#include "Container.h"

#include "Text.h"

#include <utility>

#define INTERCHAR_SIZE 2

Widget::Text::Text(const std::string &id, std::string text, const FONT_INFO *fontInfo, Graphics::Vec2D position,
                   const Graphics::Color &textColor, const Graphics::Color &backgroundColor)
: BaseWidget(id, position)
, m_text(std::move(text))
, m_fontInfo(fontInfo)
, m_horizontalAlignment(HorizontalAlignment::Left)
, m_wrapEnabled(false)
, m_size({0, 0})
, m_clippingEnabled(false)
, m_clippingStart({0, 0})
, m_clippingEnd({0, 0})
, m_lastDrawPosition({0, 0})
, m_lastSize({0, 0})
, m_textColor(textColor)
, m_backgroundColor(backgroundColor)
, m_width(0)
{
    m_fontInfo = fontInfo;
}

void Widget::Text::draw(Hardware::Screen::BaseScreen &target)
{
    // Dirty flag not set, we don't need to redraw the widget
    if (m_dirty.none()) return;

    // Setup the GfxUtils2 instance
    std::unique_ptr<Graphics::GfxUtils2> gfxTarget = std::make_unique<Graphics::GfxUtils2>(target);
    gfxTarget->setFillColor(m_backgroundColor);
    gfxTarget->setBackgroundColor(m_backgroundColor);
    gfxTarget->setLoopVerticalAxis(m_loopVerticalPosition);

    if (m_clippingEnabled)
    {
        gfxTarget->setClippingEnabled(true);
        gfxTarget->setClippingStart(m_clippingStart);
        gfxTarget->setClippingEnd(m_clippingEnd);

        // Check if clipping has changed since last time
        if (isDirty(DirtyState::User1))
        {
            if (m_clippingStart.x < m_oldClippingStart.x)
            {
                gfxTarget->drawFilledRectangle(m_clippingStart, {
                    static_cast<int16_t>(m_oldClippingStart.x - m_clippingStart.x),
                    static_cast<int16_t>(m_clippingEnd.y - m_clippingStart.y)
                });
            }

            if (m_clippingStart.y < m_oldClippingStart.y)
            {
                gfxTarget->drawFilledRectangle(m_clippingStart, {
                        static_cast<int16_t>(m_clippingEnd.x - m_clippingStart.x),
                        static_cast<int16_t>(m_oldClippingStart.y - m_clippingStart.y)
                });
            }

            if (m_clippingEnd.x > m_oldClippingEnd.x)
            {
                gfxTarget->drawFilledRectangle({m_oldClippingEnd.x, m_clippingStart.y}, {
                        static_cast<int16_t>(m_clippingEnd.x - m_oldClippingEnd.x),
                        static_cast<int16_t>(m_clippingEnd.y - m_clippingStart.y)
                });
            }

            if (m_clippingEnd.y > m_oldClippingEnd.y)
            {
                gfxTarget->drawFilledRectangle({m_clippingStart.x, m_oldClippingEnd.y}, {
                        static_cast<int16_t>(m_clippingEnd.x - m_clippingStart.x),
                        static_cast<int16_t>(m_clippingEnd.y - m_oldClippingEnd.y)
                });
            }

            // Check if we're exclusively here because clipping has changed
            if (m_dirty.count() == 1)
            {
                if (m_clippingStart.y < m_oldClippingStart.y && m_clippingEnd.y <= m_oldClippingEnd.y)
                {
                    gfxTarget->setClippingEnd({m_clippingEnd.x, m_oldClippingStart.y});
                }
                else if (m_clippingEnd.y > m_oldClippingEnd.y && m_clippingStart.y >= m_oldClippingStart.y)
                {
                    gfxTarget->setClippingStart({m_clippingStart.x, m_oldClippingEnd.y});
                }
            }
        }
    }

    // Geometry has changed, we need to clear the last occupied space
    if (isDirty(DirtyState::Global) || isDirty(DirtyState::Position) || isDirty(DirtyState::Size))
    {
        Graphics::Vec2D lastAbsolutePosition = getLastAbsolutePosition();
        gfxTarget->drawFilledRectangle(lastAbsolutePosition, m_lastSize);
    }

    // Recompute width if it's dirty
    if (isDirty(DirtyState::Global) || isDirty(DirtyState::Size))
        m_width = computeWidth();

    gfxTarget->setFillColor(m_textColor);
    drawAndGetSize(gfxTarget.get(), m_size);

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

Graphics::Vec2D Widget::Text::getAbsolutePosition() const
{
    if (m_parent == nullptr)
        return getPosition();

    return m_parent->getAbsolutePosition() + getPosition();
}

Graphics::Vec2D Widget::Text::getDrawPosition() const
{
    Graphics::Vec2D position = getAbsolutePosition();

    switch (m_horizontalAlignment)
    {
        case HorizontalAlignment::Centered:
            return {static_cast<int16_t>(position.x - getWidth() / 2), position.y};
        case HorizontalAlignment::Right:
            return {static_cast<int16_t>(position.x - getWidth()), position.y};
        default:
            return position;
    }
}

int16_t Widget::Text::getWidth() const
{
    return m_width;
}

int16_t Widget::Text::getHeight() const
{
    return m_fontInfo->height;
}

Graphics::Vec2D Widget::Text::getSize() const
{
    return {getWidth(), getHeight()};
}

void Widget::Text::setTextColor(const Graphics::Color &textColor)
{
    m_textColor = textColor;

    setDirty(DirtyState::Color, true);
}

const Graphics::Color &Widget::Text::getTextColor() const
{
    return m_textColor;
}

void Widget::Text::setBackgroundColor(const Graphics::Color &backgroundColor)
{
    m_backgroundColor = backgroundColor;

    setDirty(DirtyState::Color, true);
}

const Graphics::Color &Widget::Text::getBackgroundColor() const
{
    return m_backgroundColor;
}

void Widget::Text::setHorizontalAlignment(Widget::Text::HorizontalAlignment horizontalAlignment)
{
    m_horizontalAlignment = horizontalAlignment;

    setDirty(DirtyState::Position, true);
}

Widget::Text::HorizontalAlignment Widget::Text::getHorizontalAlignment() const
{
    return m_horizontalAlignment;
}

void Widget::Text::setWrapEnabled(bool wrapEnabled)
{
    m_wrapEnabled = wrapEnabled;
    setDirty(DirtyState::Global, true);
}

bool Widget::Text::isWrapEnabled() const
{
    return m_wrapEnabled;
}

void Widget::Text::setClippingEnabled(bool clippingEnabled)
{
    m_clippingEnabled = clippingEnabled;
    setDirty(DirtyState::Global, true);
}

bool Widget::Text::isClippingEnabled() const
{
    return m_clippingEnabled;
}

void Widget::Text::setClippingStart(const Graphics::Vec2D &clippingStart)
{
    m_oldClippingStart = m_clippingStart;
    m_clippingStart = clippingStart;

    setDirty(DirtyState::User1, true);
}

const Graphics::Vec2D &Widget::Text::getClippingStart() const
{
    return m_clippingStart;
}

void Widget::Text::setClippingEnd(const Graphics::Vec2D &clippingEnd)
{
    m_oldClippingEnd = m_clippingEnd;
    m_clippingEnd = clippingEnd;

    setDirty(DirtyState::User1, true);
}

const Graphics::Vec2D &Widget::Text::getClippingEnd() const
{
    return m_clippingEnd;
}

Graphics::Vec2D Widget::Text::getLastAbsolutePosition() const
{
    if (m_parent == nullptr)
        return m_lastDrawPosition;

    return m_parent->getAbsolutePosition() + m_lastDrawPosition;
}

uint16_t Widget::Text::computeWidth(const std::string &str) const
{
    // Prepare the width
    uint16_t width = 0;

    // Loop through all the characters and count their width
    for (const char c : str)
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

uint16_t Widget::Text::computeWidth() const
{
    return computeWidth(m_text);
}

void Widget::Text::drawAndGetSize(Graphics::GfxUtils2 *gfxTarget, Graphics::Vec2D &size)
{
    // Store the position of this drawing
    const Graphics::Vec2D baseDrawPosition = getDrawPosition();
    Graphics::Vec2D position = baseDrawPosition;

    // Save last draw position, take starting height into account
    if (!m_clippingEnabled)
        m_lastDrawPosition = position;
    else
        m_lastDrawPosition = m_clippingStart;

    int16_t maxX = position.x;

    // Draw the text depending on the wrapping mode
    if (m_wrapEnabled)
    {
        size_t i = 0;
        size_t prevI = 0;

        do
        {
            // Find the next space character
            i = m_text.find(' ', i);
            if (i == std::string::npos)
                i = m_text.size();
            else
                ++i;

            // Extract the next word and check if it needs to be on a new line
            const std::string subtext = m_text.substr(prevI, i - prevI);
            if (m_clippingEnabled && position.x + computeWidth(subtext) >= m_clippingEnd.x)
            {
                maxX = std::max(position.x, maxX);
                position.x = baseDrawPosition.x;

                position.y += m_fontInfo->height;
                if (m_clippingEnabled && position.y >= m_clippingEnd.y)
                    break;
            }

            // Draw the word
            drawStringAt(gfxTarget, subtext, position, maxX, baseDrawPosition);

            prevI = i;
        } while (i < m_text.size());
    }
    else
    {
        // Draw the word
        drawStringAt(gfxTarget, m_text, position, maxX, baseDrawPosition);
    }

    // Store the size of this drawing
    if (m_clippingEnabled)
    {
        m_lastSize = {
                static_cast<int16_t>(std::min(maxX, m_clippingEnd.x) - std::max(baseDrawPosition.x, m_clippingStart.x)),
                static_cast<int16_t>(std::min(static_cast<int16_t>(position.y + m_fontInfo->height), m_clippingEnd.y) - std::max(baseDrawPosition.y, m_clippingStart.y))
        };
    }
    else
    {
        m_lastSize = {
                static_cast<int16_t>(maxX - baseDrawPosition.x),
                static_cast<int16_t>(position.y - baseDrawPosition.y + m_fontInfo->height)
        };
    }
}

void Widget::Text::drawStringAt(Graphics::GfxUtils2 *gfxTarget, const std::string &str, Graphics::Vec2D &position,
                                int16_t &maxCursorX, const Graphics::Vec2D &basePosition)
{
    for (const char c : str)
    {
        if (c != ' ')
        {
            // Character is not a space, draw it
            Graphics::Vec2D glyphGeometry = {};
            Graphics::GfxUtils2::getCharGeometry(glyphGeometry, c, *m_fontInfo);

            if (gfxTarget != nullptr)
                gfxTarget->drawChar(position, c, *m_fontInfo);

            position.x += glyphGeometry.x;
        }
        else
        {
            // Character is a space, move forward the cursor
            position.x += m_fontInfo->spacePixels;
        }

        // Add some space between characters
        position.x += INTERCHAR_SIZE;

        // Check if we reached the size limit
        if (m_clippingEnabled && position.x >= m_clippingEnd.x)
        {
            if (m_wrapEnabled)
            {
                // Wrap the text
                maxCursorX = std::max(maxCursorX, position.x);
                position.x = basePosition.x;

                position.y += m_fontInfo->height;
                if (m_clippingEnabled && position.y >= m_clippingEnd.y)
                    break;
            }
            else
            {
                // Exit the loop
                break;
            }
        }
    }

    // Update the maxCursorX one last time
    maxCursorX = std::max(maxCursorX, position.x);
}
