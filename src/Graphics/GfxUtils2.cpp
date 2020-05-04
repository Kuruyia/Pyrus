#include "GfxUtils2.h"

#define BUFFER_SIZE 254

Graphics::GfxUtils2::GfxUtils2(Hardware::Screen::BaseScreen &target, const Graphics::Color &fillColor,
        const Graphics::Color &backgroundColor)
: m_target(target)
, m_fillColor(fillColor)
, m_backgroundColor(backgroundColor)
, m_loopVerticalAxis(false)
, m_clippingEnabled(false)
, m_clippingStart({0, 0})
, m_clippingEnd({0, 0})
{

}

const Graphics::Color &Graphics::GfxUtils2::getFillColor() const
{
    return m_fillColor;
}

void Graphics::GfxUtils2::setFillColor(const Graphics::Color &fillColor)
{
    m_fillColor = fillColor;
}

const Graphics::Color &Graphics::GfxUtils2::getBackgroundColor() const
{
    return m_backgroundColor;
}

void Graphics::GfxUtils2::setBackgroundColor(const Graphics::Color &backgroundColor)
{
    m_backgroundColor = backgroundColor;
}

bool Graphics::GfxUtils2::isLoopVerticalAxis() const
{
    return m_loopVerticalAxis;
}

void Graphics::GfxUtils2::setLoopVerticalAxis(bool loopVerticalAxis)
{
    m_loopVerticalAxis = loopVerticalAxis;
}

bool Graphics::GfxUtils2::isClippingEnabled() const
{
    return m_clippingEnabled;
}

void Graphics::GfxUtils2::setClippingEnabled(bool clippingEnabled)
{
    m_clippingEnabled = clippingEnabled;
}

const Graphics::Vec2D &Graphics::GfxUtils2::getClippingStart() const
{
    return m_clippingStart;
}

void Graphics::GfxUtils2::setClippingStart(const Graphics::Vec2D &clippingStart)
{
    m_clippingStart = clippingStart;
}

const Graphics::Vec2D &Graphics::GfxUtils2::getClippingEnd() const
{
    return m_clippingEnd;
}

void Graphics::GfxUtils2::setClippingEnd(const Graphics::Vec2D &clippingEnd)
{
    m_clippingEnd = clippingEnd;
}

void Graphics::GfxUtils2::drawChar(Graphics::Vec2D position, char c, const FONT_INFO &fontInfo)
{
    // Check if the char is supported
    if (c < fontInfo.startChar || c > fontInfo.endChar)
        c = '?';

    // Set the window
    const size_t descriptorOffset = c - fontInfo.startChar;
    const FONT_CHAR_INFO descriptor = fontInfo.charInfo[descriptorOffset];
    Graphics::Vec2D glyphSize = {descriptor.widthBits, fontInfo.height};

    const Graphics::Vec2D framebufferSize = m_target.getFramebufferSize();
    const uint8_t pixelSize = m_target.getPixelSize();

    // Check if the character will be seen on the X axis
    if (position.x < -descriptor.widthBits || position.x > framebufferSize.x)
        return;

    // Check if the character will be seen on the Y axis
    if ((position.y < -fontInfo.height || position.y > framebufferSize.y) && !m_loopVerticalAxis)
        return;

    if (m_loopVerticalAxis)
        position.y %= framebufferSize.y;

    m_target.setWindow(position, glyphSize);

    // Get raw color
    uint32_t rawTextColor = m_target.convertColorToRaw(m_fillColor);
    uint32_t rawBackgroundColor = m_target.convertColorToRaw(m_backgroundColor);

    // Prepare the Screen controller to receive data
    m_target.prepareDrawBuffer();

    // Allocate the buffer
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);

    // Check if the glyph will overflow on the X axis
    uint16_t overflowX = 0;
    if (position.x + descriptor.widthBits > framebufferSize.x)
    {
        overflowX = position.x + descriptor.widthBits - framebufferSize.x;
        glyphSize.x -= overflowX;
    }

    // Calculate the geometry of this glyph
    const uint16_t pixelCount = glyphSize.x * glyphSize.y;
    const uint8_t bytesPerLine = (descriptor.widthBits % 8 > 0) ? descriptor.widthBits / 8 + 1 : descriptor.widthBits / 8;

    // Calculate how many parts we must send to the screen
    const size_t totalBufferSize = pixelCount * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Those hold the current position of the cursor
    size_t actualPixel = 0;
    size_t actualPixelInGlyph = 0;
    Graphics::Vec2D actualPosition = position;

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Prepare the buffer to send data to the Screen
        const size_t endPixel = (actualStep == numberOfStep - 1) ? pixelCount : (BUFFER_SIZE / pixelSize) * (actualStep + 1);
        size_t bufferPos = 0;

        // Read data from the font definition
        while (actualPixel < endPixel)
        {
            const uint8_t bitInBlock = actualPixelInGlyph % descriptor.widthBits;
            if (bitInBlock < glyphSize.x)
            {
                const size_t actualByte = (bitInBlock / 8) + (bytesPerLine * (actualPixelInGlyph / descriptor.widthBits));
                const uint8_t actualVal = fontInfo.data[descriptor.offset + actualByte] & (1 << (bitInBlock % 8));

                if (actualVal)
                    bufferPos = m_target.putPixelInBuffer(buffer, rawTextColor, bufferPos);
                else
                    bufferPos = m_target.putPixelInBuffer(buffer, rawBackgroundColor, bufferPos);

                ++actualPixel;
                ++actualPixelInGlyph;
            }
            else
            {
                actualPixelInGlyph += overflowX;
            }
        }

        // Draw the buffer
        const bool mustContinue = m_target.drawBuffer(position, glyphSize, actualPixel, actualPosition, buffer,
                                                    bufferPos / pixelSize, verticalLoopCount, m_loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

void Graphics::GfxUtils2::getCharGeometry(Graphics::Vec2D &geometry, char c, const FONT_INFO &fontInfo)
{
    // Check if the char is supported
    if (c < fontInfo.startChar || c > fontInfo.endChar)
        c = '?';

    const size_t descriptorOffset = c - fontInfo.startChar;
    const FONT_CHAR_INFO descriptor = fontInfo.charInfo[descriptorOffset];
    geometry = {descriptor.widthBits, fontInfo.height};
}
