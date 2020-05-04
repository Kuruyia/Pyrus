#include "../Graphics/GfxUtils.h"
#include "Container.h"

#include "Text.h"

#define INTERCHAR_SIZE 2

Widget::Text::Text(const std::string &id, const std::string &text, const FONT_INFO *fontInfo, Graphics::Vec2D position,
                   const Graphics::Color &textColor, const Graphics::Color &backgroundColor)
: BaseWidget(id, position)
, m_text(text)
, m_fontInfo(fontInfo)
, m_horizontalAlignment(HorizontalAlignment::Left)
, m_wrapMode(WrapMode::None)
, m_sizeLimit({0, 0})
, m_size({0, 0})
, m_oldStartHeight(0)
, m_startHeight(0)
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

    // Recompute width if it's dirty
    if (isDirty(DirtyState::Global) || isDirty(DirtyState::Size))
        m_width = computeWidth();

    drawAndGetSize(&target, m_size);

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

void Widget::Text::setWrapMode(Widget::Text::WrapMode wrapMode)
{
    m_wrapMode = wrapMode;
    setDirty(DirtyState::Global, true);
}

Widget::Text::WrapMode Widget::Text::getWrapMode() const
{
    return m_wrapMode;
}

void Widget::Text::setSizeLimit(const Graphics::Vec2D &sizeLimit)
{
    m_sizeLimit = sizeLimit;
    setDirty(DirtyState::Size, true);
}

const Graphics::Vec2D &Widget::Text::getSizeLimit() const
{
    return m_sizeLimit;
}

void Widget::Text::setStartHeight(uint16_t startHeight)
{
    // Only set dirty if the change would be visible
    if (m_startHeight / m_fontInfo->height != startHeight / m_fontInfo->height)
        setDirty(DirtyState::Global, true);

    m_oldStartHeight = m_startHeight;
    m_startHeight = startHeight;
}

uint16_t Widget::Text::getStartHeight() const
{
    return m_startHeight;
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

void Widget::Text::drawAndGetSize(Hardware::Screen::BaseScreen *target, Graphics::Vec2D &size)
{
    // Store the position of this drawing
    const Graphics::Vec2D baseDrawPosition = getDrawPosition();
    Graphics::Vec2D position = baseDrawPosition;

    // Save last draw position, take starting height into account
    m_lastDrawPosition = position;
    m_lastDrawPosition.y += m_startHeight;

    // Loop the vertical axis if enabled
    if (m_loopVerticalPosition && target != nullptr)
        position.y %= target->getFramebufferSize().y;

    // Draw the text depending on the wrapping mode
    if (m_wrapMode == WrapMode::Wrap)
    {
        int16_t maxX = position.x;
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
            if (m_sizeLimit.x > 0 && computeWidth(subtext) + (position.x - baseDrawPosition.x) >= m_sizeLimit.x)
            {
                maxX = std::max(position.x, maxX);
                position.x = baseDrawPosition.x;

                position.y += m_fontInfo->height;
                if (m_sizeLimit.y > 0 && m_fontInfo->height + (position.y - baseDrawPosition.y) >= m_sizeLimit.y + m_startHeight)
                    break;
            }

            // Draw the word
            drawStringAt(target, subtext, position, maxX, baseDrawPosition);

            prevI = i;
        } while (i < m_text.size() && (m_sizeLimit.y == 0 || m_fontInfo->height + (position.y - baseDrawPosition.y) < m_sizeLimit.y + m_startHeight));

        // Store the size of this drawing
        // Note that m_lastDrawPosition is used here because it holds the initial values of position
        m_lastSize = {static_cast<int16_t>(maxX - m_lastDrawPosition.x), static_cast<int16_t>(319)};
    }
    else
    {
        // Loop through all the characters and draw them
        for (const char c : m_text)
        {
            if (c != ' ')
            {
                Graphics::Vec2D glyphGeometry = {};
                Graphics::GfxUtils::getCharGeometry(glyphGeometry, c, *m_fontInfo);

                if (target != nullptr)
                    Graphics::GfxUtils::drawChar(*target, position, c, *m_fontInfo, m_textColor,
                                                 m_backgroundColor, m_loopVerticalPosition);

                position.x += glyphGeometry.x;
            }
            else
            {
                position.x += m_fontInfo->spacePixels;
            }

            position.x += INTERCHAR_SIZE;
        }

        // Store the size of this drawing
        // Note that m_lastDrawPosition is used here because it holds the initial values of position
        m_lastSize = {static_cast<int16_t>(position.x - m_lastDrawPosition.x), m_fontInfo->height};
    }
}

void Widget::Text::drawStringAt(Hardware::Screen::BaseScreen *target, const std::string &str, Graphics::Vec2D &position,
                                int16_t &maxCursorX, const Graphics::Vec2D &basePosition)
{
    for (const char c : str)
    {
        if (c != ' ')
        {
            // Character is not a space, draw it
            Graphics::Vec2D glyphGeometry = {};
            Graphics::GfxUtils::getCharGeometry(glyphGeometry, c, *m_fontInfo);

            if (position.y - basePosition.y >= m_startHeight - m_fontInfo->height && target != nullptr)
                Graphics::GfxUtils::drawChar(*target, position, c, *m_fontInfo, m_textColor,
                                             m_backgroundColor, m_loopVerticalPosition);

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
        if (m_sizeLimit.x > 0 && position.x - basePosition.x >= m_sizeLimit.x)
        {
            if (m_wrapMode == WrapMode::Wrap)
            {
                // Wrap the text
                maxCursorX = std::max(maxCursorX, position.x);
                position.x = basePosition.x;

                position.y += m_fontInfo->height;
                if (m_sizeLimit.y > 0 && m_fontInfo->height + (position.y - basePosition.y) >= m_sizeLimit.y + m_startHeight)
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
