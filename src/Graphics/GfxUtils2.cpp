#include <algorithm>

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
    const Graphics::Vec2D glyphSize = {descriptor.widthBits, fontInfo.height};
    Graphics::Vec2D drawSize = glyphSize;

    const Graphics::Vec2D framebufferSize = m_target.getFramebufferSize();
    const uint8_t pixelSize = m_target.getPixelSize();
    Graphics::Vec2D correctedBasePosition = {
            position.x,
            static_cast<int16_t>(m_loopVerticalAxis ? position.y % framebufferSize.y : position.y)
    };

    // Get the drawing window
    Graphics::Vec2D windowStart = {};
    Graphics::Vec2D windowEnd = {};

    getDrawingWindow(windowStart, windowEnd);

    // Check if the character will be seen on the X axis
    if (position.x < windowStart.x - descriptor.widthBits || position.x > windowEnd.x)
        return;

    // Check if the character will be seen on the Y axis
    if (position.y < windowStart.y - fontInfo.height || (position.y > windowEnd.y && (!m_loopVerticalAxis || m_clippingEnabled)))
        return;

    // Check if the glyph will overflow on the X axis
    uint16_t overflowRightX = 0;
    if (position.x + descriptor.widthBits > windowEnd.x)
    {
        overflowRightX = position.x + descriptor.widthBits - windowEnd.x;
        drawSize.x -= overflowRightX;
    }

    uint16_t overflowLeftX = 0;
    if (position.x < windowStart.x)
    {
        overflowLeftX = windowStart.x - position.x;

        position.x += overflowLeftX;
        drawSize.x -= overflowLeftX;
    }

    // Check if the glyph will overflow on the Y axis
    uint16_t overflowBottomY = 0;
    if (position.y + fontInfo.height > windowEnd.y)
    {
        overflowBottomY = position.y + fontInfo.height - windowEnd.y;
        drawSize.y -= overflowBottomY;
    }

    uint16_t overflowTopY = 0;
    if (position.y < windowStart.y)
    {
        overflowTopY = windowStart.y - position.y;

        correctedBasePosition.y += overflowTopY;
        drawSize.y -= overflowTopY;
    }

    // Set the proper window
    position.y = correctedBasePosition.y;
    m_target.setWindow(position, drawSize);

    // Get raw color
    uint32_t rawTextColor = m_target.convertColorToRaw(m_fillColor);
    uint32_t rawBackgroundColor = m_target.convertColorToRaw(m_backgroundColor);

    // Prepare the Screen controller to receive data
    m_target.prepareDrawBuffer();

    // Allocate the buffer
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);

    // Calculate the geometry of this glyph
    const uint16_t pixelCount = drawSize.x * drawSize.y;
    const uint8_t bytesPerLine = (descriptor.widthBits % 8 > 0) ? descriptor.widthBits / 8 + 1 : descriptor.widthBits / 8;

    // Calculate how many parts we must send to the screen
    const size_t totalBufferSize = pixelCount * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Those hold the current position of the cursor
    size_t actualPixel = 0;
    size_t actualPixelInGlyph = descriptor.widthBits * overflowTopY;
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
            if (bitInBlock < overflowLeftX)
            {
                actualPixelInGlyph += overflowLeftX;
            }
            else if (bitInBlock >= glyphSize.x - overflowRightX)
            {
                actualPixelInGlyph += overflowRightX;
            }
            else
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
        }

        // Draw the buffer
        const bool mustContinue = m_target.drawBuffer(position, drawSize, actualPixel, actualPosition, buffer,
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

void Graphics::GfxUtils2::drawLine(Graphics::Vec2D firstPoint, Graphics::Vec2D secondPoint)
{
    // This is an implementation of the Bresenham's line algorithm

    const Graphics::Vec2D fbSize = m_target.getFramebufferSize();

    // Get the drawing window
    Graphics::Vec2D windowStart = {};
    Graphics::Vec2D windowEnd = {};

    getDrawingWindow(windowStart, windowEnd);

    // Correct the drawing window if vertical looping is enabled
    int16_t verticalLoopCount = 0;
    if (m_loopVerticalAxis)
    {
        verticalLoopCount = std::min(firstPoint.y, secondPoint.y) / fbSize.y;

        windowStart.y -= fbSize.y * verticalLoopCount;
        windowEnd.y -= fbSize.y * verticalLoopCount;
    }

    // Bresenham's algorithm parameters
    Graphics::Vec2D delta = secondPoint - firstPoint;
    Graphics::Vec2D absDelta {static_cast<int16_t>(std::abs(delta.x)), static_cast<int16_t>(std::abs(delta.y))};

    Graphics::Vec2D p {static_cast<int16_t>(2 * absDelta.y - absDelta.x),
                       static_cast<int16_t>(2 * absDelta.x - absDelta.y)};

    Graphics::Vec2D currentPosition {};
    int16_t endPosition = -fbSize.y * verticalLoopCount;

    // Bresenham's algorithm
    if (absDelta.y <= absDelta.x)
    {
        // Check which point to begin with
        if (delta.x >= 0)
        {
            currentPosition = firstPoint;
            endPosition += secondPoint.x;
        }
        else
        {
            currentPosition = secondPoint;
            endPosition += firstPoint.x;
        }

        // We ensure the Y coordinate is in bounds, according to the vertical looping policy
        if (currentPosition.y >= fbSize.y)
        {
            if (m_loopVerticalAxis)
                currentPosition.y %= fbSize.y;
            else
                return;
        }

        // Draw all the pixels until we reach the end X coordinate
        for (int16_t i = 0; currentPosition.x < endPosition; ++i)
        {
            if (currentPosition.x >= windowStart.x && currentPosition.x <= windowEnd.x &&
                    currentPosition.y >= windowStart.y && currentPosition.y <= windowEnd.y)
            {
                m_target.drawPixel(currentPosition, m_fillColor);
            }

            ++currentPosition.x;

            if (p.x < 0)
            {
                p.x = p.x + 2 * absDelta.y;
            }
            else
            {
                if ((delta.x < 0 && delta.y < 0) || (delta.x > 0 && delta.y > 0))
                {
                    // Increment the Y coordinate and check if it's still in bounds
                    if (++currentPosition.y >= fbSize.y)
                    {
                        if (m_loopVerticalAxis)
                        {
                            currentPosition.y = 0;
                            windowStart.y -= fbSize.y;
                            windowEnd.y -= fbSize.y;
                        }
                        else
                        {
                            return;
                        }
                    }
                }
                else
                {
                    --currentPosition.y;
                }

                p.x = p.x + 2 * (absDelta.y - absDelta.x);
            }
        }
    }
    else
    {
        // Check which point to begin with
        if (delta.y >= 0)
        {
            currentPosition = firstPoint;
            endPosition += secondPoint.y;
        }
        else
        {
            currentPosition = secondPoint;
            endPosition += firstPoint.y;
        }

        // We ensure the Y coordinate is in bounds, according to the vertical looping policy
        if (currentPosition.y >= fbSize.y)
        {
            if (m_loopVerticalAxis)
                currentPosition.y %= fbSize.y;
            else
                return;
        }

        // Draw all the pixels until we reach the end X coordinate
        for (int16_t i = 0; currentPosition.y < endPosition; ++i)
        {
            if (currentPosition.x >= windowStart.x && currentPosition.x <= windowEnd.x &&
                    currentPosition.y >= windowStart.y && currentPosition.y <= windowEnd.y)
            {
                m_target.drawPixel(currentPosition, m_fillColor);
            }

            // Increment the Y coordinate and check if it's still in bounds
            if (++currentPosition.y >= fbSize.y)
            {
                if (m_loopVerticalAxis)
                {
                    currentPosition.y = 0;
                    windowStart.y -= fbSize.y;
                    windowEnd.y -= fbSize.y;
                }
                else
                {
                    return;
                }
            }

            if (p.y <= 0)
            {
                p.y = p.y + 2 * absDelta.x;
            }
            else
            {
                if ((delta.x < 0 && delta.y < 0) || (delta.x > 0 && delta.y > 0))
                    ++currentPosition.x;
                else
                    --currentPosition.x;

                p.y = p.y + 2 * (absDelta.x - absDelta.y);
            }
        }
    }
}

void Graphics::GfxUtils2::drawHorizontalLine(const Graphics::Vec2D &position, uint16_t width)
{
    drawFilledRectangle(position, {static_cast<int16_t>(width), 1});
}

void Graphics::GfxUtils2::drawVerticalLine(const Graphics::Vec2D &position, uint16_t height)
{
    drawFilledRectangle(position, {1, static_cast<int16_t>(height)});
}

void Graphics::GfxUtils2::drawFilledRectangle(Graphics::Vec2D position, Graphics::Vec2D size)
{
    const Vec2D fbSize = m_target.getFramebufferSize();

    // Get the drawing window
    Graphics::Vec2D windowStart = {};
    Graphics::Vec2D windowEnd = {};
    getDrawingWindow(windowStart, windowEnd);

    // Check if the character will be seen on the X axis
    if (position.x < windowStart.x - size.x || position.x > windowEnd.x)
        return;

    // Check if the character will be seen on the Y axis
    if (position.y < windowStart.y - size.y || (position.y > windowEnd.y && (!m_loopVerticalAxis || m_clippingEnabled)))
        return;

    // Fix the position and size
    if (position.x + size.x > windowEnd.x)
    {
        size.x = windowEnd.x - position.x;
    }

    if (position.x < windowStart.x)
    {
        size.x -= windowStart.x - position.x;
        position.x = windowStart.x;
    }

    if (position.y + size.y > windowEnd.y)
    {
        size.y = windowEnd.y - position.y;
    }

    if (position.y < windowStart.y)
    {
        size.y -= windowStart.y - position.y;
        position.y = windowStart.y;
    }

    // Check that the Y coordinate is in bounds
    if (position.y >= fbSize.y)
    {
        if (m_loopVerticalAxis)
            position.y %= fbSize.y;
        else
            return;
    }

    // Set the window
    m_target.setWindow(position, size);

    // Get some properties from the target
    uint32_t rawColor = m_target.convertColorToRaw(m_fillColor);

    // Calculate how many parts we must send to the screen
    const uint8_t pixelSize = m_target.getPixelSize();
    const size_t totalBufferSize = size.x * size.y * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Keep track of the current position
    size_t actualPixel = 0;
    Graphics::Vec2D actualPosition = position;

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    // Prepare a large buffer of pixels for faster transmission
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);
    fillBufferWithColor(buffer, BUFFER_SIZE, rawColor);

    m_target.prepareDrawBuffer();

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Calculate the number of pixels to feed to the screen and the next cursor position
        const size_t pixelsToFeed = (actualStep == numberOfStep - 1) ? (size.x * size.y) % (BUFFER_SIZE / pixelSize) : BUFFER_SIZE / pixelSize;
        actualPixel += pixelsToFeed;

        // Draw the buffer
        const bool mustContinue = m_target.drawBuffer(position, size, actualPixel, actualPosition, buffer,
                                                      pixelsToFeed,verticalLoopCount, m_loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

void Graphics::GfxUtils2::drawTriangle(Graphics::Vec2D firstPoint, Graphics::Vec2D secondPoint,
                                       Graphics::Vec2D thirdPoint)
{
    drawLine(firstPoint, secondPoint);
    drawLine(firstPoint, thirdPoint);
    drawLine(secondPoint, thirdPoint);
}

void Graphics::GfxUtils2::drawFilledTriangle(Graphics::Vec2D firstPoint, Graphics::Vec2D secondPoint,
                                             Graphics::Vec2D thirdPoint)
{
    // This is an adaptation of Adafruit-GFX's filled triangle drawing algorithm

    int16_t a, b, y, last;

    // Reorder the points
    if (thirdPoint.y < secondPoint.y)
        std::swap(secondPoint, thirdPoint);
    if (secondPoint.y < firstPoint.y)
        std::swap(firstPoint, secondPoint);
    if (thirdPoint.y < secondPoint.y)
        std::swap(secondPoint, thirdPoint);

    const Graphics::Vec2D d01 = secondPoint - firstPoint;
    const Graphics::Vec2D d02 = thirdPoint - firstPoint;
    const Graphics::Vec2D d12 = thirdPoint - secondPoint;
    int32_t sa = 0;
    int32_t sb = 0;

    if (secondPoint.y == thirdPoint.y)
        last = secondPoint.y;
    else
        last = secondPoint.y - 1;

    y = firstPoint.y;
    while (y <= last)
    {
        a = firstPoint.x + sa / d01.y;
        b = firstPoint.x + sb / d02.y;
        sa += d01.x;
        sb += d02.x;

        if (a > b)
            std::swap(a, b);

        drawHorizontalLine({a, y}, b - a + 1);

        ++y;
    }

    sa = (int32_t)d12.x * (y - secondPoint.y);
    sb = (int32_t)d02.x * (y - firstPoint.y);
    while (y <= thirdPoint.y)
    {
        a = secondPoint.x + sa / d12.y;
        b = firstPoint.x + sb / d02.y;
        sa += d12.x;
        sb += d02.x;

        if (a > b)
            std::swap(a, b);

        drawHorizontalLine({a, y}, b - a + 1);

        ++y;
    }
}

void Graphics::GfxUtils2::drawFilledCircle(const Graphics::Vec2D &origin, const uint16_t radius)
{
    // This is an implementation of https://stackoverflow.com/a/1237519

    const Graphics::Vec2D fbSize = m_target.getFramebufferSize();

    // Compute circle bounds
    Graphics::Vec2D drawSize = {static_cast<int16_t>(radius*2), static_cast<int16_t>(radius*2)};
    Graphics::Vec2D position = {static_cast<int16_t>(origin.x - radius), static_cast<int16_t>(origin.y - radius)};

    // Get the drawing window
    Graphics::Vec2D windowStart = {};
    Graphics::Vec2D windowEnd = {};
    getDrawingWindow(windowStart, windowEnd);

    // Check if the character will be seen on the X axis
    if (origin.x + radius < windowStart.x || origin.x - radius > windowEnd.x)
        return;

    // Check if the character will be seen on the Y axis
    if (origin.y + radius < windowStart.y || (origin.y - radius > windowEnd.y && (!m_loopVerticalAxis || m_clippingEnabled)))
        return;

    // Check if the circle will overflow on the X axis
    uint16_t overflowRightX = 0;
    if (origin.x + radius > windowEnd.x)
    {
        overflowRightX = origin.x + radius - windowEnd.x;
        drawSize.x -= overflowRightX;
    }

    uint16_t overflowLeftX = 0;
    if (origin.x - radius < windowStart.x)
    {
        overflowLeftX = windowStart.x - origin.x + radius;

        position.x += overflowLeftX;
        drawSize.x -= overflowLeftX;
    }

    // Check if the circle will overflow on the Y axis
    uint16_t overflowBottomY = 0;
    if (origin.y + radius > windowEnd.y)
    {
        overflowBottomY = origin.y + radius - windowEnd.y;
        drawSize.y -= overflowBottomY;
    }

    uint16_t overflowTopY = 0;
    if (origin.y - radius < windowStart.y)
    {
        overflowTopY = windowStart.y - origin.y + radius;

        position.y += overflowTopY;
        drawSize.y -= overflowTopY;
    }

    int16_t overflowLeftFromRadius = -radius + overflowLeftX;
    int16_t overflowRightFromRadius = radius - overflowRightX;

    // Correct the Y axis if vertical looping enabled
    if (m_loopVerticalAxis)
        position.y %= fbSize.y;

    // Set the window
    m_target.setWindow(position, drawSize);

    const uint32_t squareRadius = radius * radius;

    // Get some properties from the target
    uint32_t rawCircleColor = m_target.convertColorToRaw(m_fillColor);
    uint32_t rawBackgroundColor = m_target.convertColorToRaw(m_backgroundColor);

    // Calculate how many parts we must send to the screen
    const uint8_t pixelSize = m_target.getPixelSize();
    const size_t totalBufferSize = drawSize.x * drawSize.y * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Keep track of the current position
    size_t actualPixel = 0;
    Graphics::Vec2D actualPosition = position;
    Graphics::Vec2D cursorPositionFromOrigin = {static_cast<int16_t>(-radius), static_cast<int16_t>(-radius + overflowTopY)};

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    // Prepare a large buffer of pixels for faster transmission
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);

    m_target.prepareDrawBuffer();

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Calculate the number of pixels to feed to the screen and the next cursor position
        const size_t pixelsToFeed = (actualStep == numberOfStep - 1) ? (drawSize.x * drawSize.y) % (BUFFER_SIZE / pixelSize) : BUFFER_SIZE / pixelSize;
        size_t bufferPos = 0;

        for (size_t i = 0; i < pixelsToFeed; ++i)
        {
            // Fix cursor position if needed
            if (cursorPositionFromOrigin.x < overflowLeftFromRadius)
                cursorPositionFromOrigin.x = overflowLeftFromRadius;

            // Check if the cursor is inside the circle
            if (static_cast<uint16_t>(cursorPositionFromOrigin.x * cursorPositionFromOrigin.x) + static_cast<uint16_t>(cursorPositionFromOrigin.y * cursorPositionFromOrigin.y) <= squareRadius + radius)
                bufferPos = m_target.putPixelInBuffer(buffer, rawCircleColor, bufferPos);
            else
                bufferPos = m_target.putPixelInBuffer(buffer, rawBackgroundColor, bufferPos);

            if (++cursorPositionFromOrigin.x >= overflowRightFromRadius)
            {
                cursorPositionFromOrigin.x = -radius;
                ++cursorPositionFromOrigin.y;
            }
        }

        // Draw the buffer
        const bool mustContinue = m_target.drawBuffer(position, drawSize, actualPixel, actualPosition, buffer,
                                                    pixelsToFeed, verticalLoopCount, m_loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

void Graphics::GfxUtils2::getDrawingWindow(Graphics::Vec2D &windowStart, Graphics::Vec2D &windowEnd)
{
    const Graphics::Vec2D fbSize = m_target.getFramebufferSize();

    if (m_clippingEnabled)
    {
        // Return the clipping window if enabled
        windowStart = {std::max((int16_t)0, m_clippingStart.x), std::max((int16_t)0, m_clippingStart.y)};
        windowEnd = {std::min(fbSize.x, m_clippingEnd.x), m_loopVerticalAxis ? m_clippingEnd.y : std::min(fbSize.y, m_clippingEnd.y)};
    }
    else
    {
        // Return the framebuffer window otherwise
        windowStart = {0, 0};
        windowEnd = {fbSize.x, static_cast<int16_t>(m_loopVerticalAxis ? INT16_MAX : fbSize.y)};
    }
}

void Graphics::GfxUtils2::fillBufferWithColor(uint8_t *buffer, size_t size, uint32_t rawColor)
{
    size_t i = 0;
    while ((i = m_target.putPixelInBuffer(buffer, rawColor, i)) < size) {}
}
