#include <algorithm>
#include <cstring>

#include "GfxUtils.h"

#define BUFFER_SIZE 254

void Graphics::GfxUtils::drawLine(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
                                  Graphics::Vec2D secondPoint, const Graphics::Color &color, bool loopVerticalAxis)
{
    // This is an implementation of the Bresenham's line algorithm

    const Graphics::Vec2D fbSize = target.getFramebufferSize();

    // Bresenham's algorithm parameters
    Graphics::Vec2D delta = secondPoint - firstPoint;
    Graphics::Vec2D absDelta {static_cast<int16_t>(std::abs(delta.x)), static_cast<int16_t>(std::abs(delta.y))};

    Graphics::Vec2D p {static_cast<int16_t>(2 * absDelta.y - absDelta.x),
                       static_cast<int16_t>(2 * absDelta.x - absDelta.y)};

    Graphics::Vec2D currentPosition {};
    Graphics::Vec2D endPosition {};

    // Bresenham's algorithm
    if (absDelta.y <= absDelta.x)
    {
        if (delta.x >= 0)
        {
            currentPosition = firstPoint;
            endPosition.x = secondPoint.x;
        }
        else
        {
            currentPosition = secondPoint;
            endPosition.x = firstPoint.x;
        }

        for (int16_t i = 0; currentPosition.x < endPosition.x; ++i)
        {
            target.drawPixel(currentPosition, color);

            ++currentPosition.x;

            if (p.x < 0)
            {
                p.x = p.x + 2 * absDelta.y;
            }
            else
            {
                if ((delta.x < 0 && delta.y < 0) || (delta.x > 0 && delta.y > 0))
                {
                    if (++currentPosition.y >= fbSize.y)
                    {
                        if (loopVerticalAxis)
                            currentPosition.y %= fbSize.y;
                        else
                            return;
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
        if (delta.y >= 0)
        {
            currentPosition = firstPoint;
            endPosition.y = secondPoint.y;
        }
        else
        {
            currentPosition = secondPoint;
            endPosition.y = firstPoint.y;
        }

        for (int16_t i = 0; currentPosition.y < endPosition.y; ++i)
        {
            target.drawPixel(currentPosition, color);

            if (++currentPosition.y >= fbSize.y)
            {
                if (loopVerticalAxis)
                    currentPosition.y %= fbSize.y;
                else
                    return;
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

void Graphics::GfxUtils::drawHorizontalLine(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
                                            uint16_t width, const Graphics::Color &color, bool loopVerticalAxis)
{
    drawFilledRectangle(target, position, {static_cast<int16_t>(width), 1}, color, loopVerticalAxis);
}

void Graphics::GfxUtils::drawVerticalLine(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
                                          uint16_t height, const Graphics::Color &color, bool loopVerticalAxis)
{
    drawFilledRectangle(target, position, {1, static_cast<int16_t>(height)}, color, loopVerticalAxis);
}

void Graphics::GfxUtils::drawFilledRectangle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
                                             const Graphics::Vec2D &size, const Graphics::Color &color, bool loopVerticalAxis)
{
    // Set the window
    target.setWindow(position, size);

    // Get some properties from the target
    uint32_t rawColor = target.convertColorToRaw(color);

    // Calculate how many parts we must send to the screen
    const uint8_t pixelSize = target.getPixelSize();
    const size_t totalBufferSize = size.x * size.y * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Keep track of the current position
    size_t actualPixel = 0;
    Graphics::Vec2D actualPosition = position;

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    // Prepare a large buffer of pixels for faster transmission
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);
    fillBufferWithColor(target, buffer, BUFFER_SIZE, rawColor);

    target.prepareDrawBuffer();

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Calculate the number of pixels to feed to the screen and the next cursor position
        const size_t pixelsToFeed = (actualStep == numberOfStep - 1) ? (size.x * size.y) % BUFFER_SIZE : BUFFER_SIZE / pixelSize;
        actualPixel += pixelsToFeed;

        // Draw the buffer
        const bool mustContinue = target.drawBuffer(position, size, actualPixel, actualPosition, buffer,
                pixelsToFeed,verticalLoopCount, loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

void Graphics::GfxUtils::drawTriangle(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
                                      Graphics::Vec2D secondPoint, Graphics::Vec2D thirdPoint,
                                      const Graphics::Color &color, bool loopVerticalAxis)
{
    drawLine(target, firstPoint, secondPoint, color, loopVerticalAxis);
    drawLine(target, firstPoint, thirdPoint, color, loopVerticalAxis);
    drawLine(target, secondPoint, thirdPoint, color, loopVerticalAxis);
}

void Graphics::GfxUtils::drawFilledTriangle(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
                                            Graphics::Vec2D secondPoint, Graphics::Vec2D thirdPoint,
                                            const Graphics::Color &triangleColor, const Graphics::Color &backgroundColor,
                                            bool loopVerticalAxis)
{
    // This is an adaptation of Adafruit-GFX's filled triangle drawing algorithm

    int16_t a, b, y, last;
    const Graphics::Vec2D fbSize = target.getFramebufferSize();

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
        int16_t drawY = y;
        if (y >= fbSize.y)
        {
            if (loopVerticalAxis)
                drawY %= fbSize.y;
            else
                return;
        }

        a = firstPoint.x + sa / d01.y;
        b = firstPoint.x + sb / d02.y;
        sa += d01.x;
        sb += d02.x;

        if (a > b)
            std::swap(a, b);

        drawHorizontalLine(target, {a, drawY}, b - a + 1, triangleColor);

        ++y;
    }

    sa = (int32_t)d12.x * (y - secondPoint.y);
    sb = (int32_t)d02.x * (y - firstPoint.y);
    while (y <= thirdPoint.y)
    {
        int16_t drawY = y;
        if (y >= fbSize.y)
        {
            if (loopVerticalAxis)
                drawY %= fbSize.y;
            else
                return;
        }

        a = secondPoint.x + sa / d12.y;
        b = firstPoint.x + sb / d02.y;
        sa += d12.x;
        sb += d02.x;

        if (a > b)
            std::swap(a, b);

        drawHorizontalLine(target, {a, drawY}, b - a + 1, triangleColor);

        ++y;
    }
}

void Graphics::GfxUtils::drawFastFilledTriangle(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
                                                Graphics::Vec2D secondPoint, Graphics::Vec2D thirdPoint,
                                                const Graphics::Color &triangleColor,
                                                const Graphics::Color &backgroundColor, bool loopVerticalAxis)
{
    // This is an adaptation of Adafruit-GFX's filled triangle drawing algorithm

    // Reorder the points
    if (thirdPoint.y < secondPoint.y)
        std::swap(secondPoint, thirdPoint);
    if (secondPoint.y < firstPoint.y)
        std::swap(firstPoint, secondPoint);
    if (thirdPoint.y < secondPoint.y)
        std::swap(secondPoint, thirdPoint);

    // Set the window
    Graphics::Vec2D position {std::min(std::min(firstPoint.x, secondPoint.x), thirdPoint.x), firstPoint.y};
    const Graphics::Vec2D maxPosition {std::max(std::max(firstPoint.x, secondPoint.x), thirdPoint.x), thirdPoint.y};
    const Graphics::Vec2D size = maxPosition - position;
    target.setWindow(position, size);

    // Get some properties from the target
    uint32_t rawTriangleColor = target.convertColorToRaw(triangleColor);
    uint32_t rawBackgroundColor = target.convertColorToRaw(backgroundColor);

    // Calculate how many parts we must send to the screen
    const uint8_t pixelSize = target.getPixelSize();
    const size_t totalBufferSize = size.x * size.y * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Keep track of the current position
    size_t actualPixel = 0;
    Graphics::Vec2D actualPosition = position;
    Graphics::Vec2D cursorPosition = position;

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    // Prepare a large buffer of pixels for faster transmission
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);

    // Algorithm parameters
    int16_t a, b, last;
    const Graphics::Vec2D d01 = secondPoint - firstPoint;
    const Graphics::Vec2D d02 = thirdPoint - firstPoint;
    const Graphics::Vec2D d12 = thirdPoint - secondPoint;
    Graphics::Vec2D activeDistance = d01;
    int16_t activeX = firstPoint.x;
    int32_t sa = 0;
    int32_t sb = 0;

    if (secondPoint.y == thirdPoint.y)
        last = secondPoint.y;
    else
        last = secondPoint.y - 1;

    a = activeX + sa / activeDistance.y;
    b = firstPoint.x + sb / d02.y;
    sa += activeDistance.x;
    sb += d02.x;

    if (a > b)
        std::swap(a, b);

    target.prepareDrawBuffer();

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Calculate the number of pixels to feed to the screen and the next cursor position
        const size_t pixelsToFeed = (actualStep == numberOfStep - 1) ? (size.x * size.y) % BUFFER_SIZE : BUFFER_SIZE / pixelSize;
        size_t bufferPos = 0;

        for (size_t i = 0; i < pixelsToFeed; ++i)
        {
            if (cursorPosition.x >= a && cursorPosition.x <= b + 1)
                bufferPos = target.putPixelInBuffer(buffer, rawTriangleColor, bufferPos);
            else
                bufferPos = target.putPixelInBuffer(buffer, rawBackgroundColor, bufferPos);

            if (++cursorPosition.x == position.x + size.x)
            {
                cursorPosition.x = position.x;
                if (++cursorPosition.y == last + 1)
                {
                    sa = (int32_t)d12.x * (cursorPosition.y - secondPoint.y);
                    sb = (int32_t)d02.x * (cursorPosition.y - firstPoint.y);

                    activeDistance = d12;
                    activeX = secondPoint.x;
                }

                a = activeX + sa / activeDistance.y;
                b = firstPoint.x + sb / d02.y;
                sa += activeDistance.x;
                sb += d02.x;

                if (a > b)
                    std::swap(a, b);
            }
        }

        // Draw the buffer
        const bool mustContinue = target.drawBuffer(position, size, actualPixel, actualPosition, buffer,
                                                    pixelsToFeed, verticalLoopCount, loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

void Graphics::GfxUtils::drawFilledCircle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &origin,
                                          uint16_t radius, const Graphics::Color &circleColor,
                                          const Graphics::Color &backgroundColor, bool loopVerticalAxis)
{
    // This is an implementation of https://stackoverflow.com/a/1237519

    // Set the window
    Graphics::Vec2D size {static_cast<int16_t>(radius*2), static_cast<int16_t>(radius*2)};
    Graphics::Vec2D position {static_cast<int16_t>(origin.x - radius), static_cast<int16_t>(origin.y - radius)};
    target.setWindow(position, size);

    const uint32_t squareRadius = radius * radius;

    // Get some properties from the target
    uint32_t rawCircleColor = target.convertColorToRaw(circleColor);
    uint32_t rawBackgroundColor = target.convertColorToRaw(backgroundColor);

    // Calculate how many parts we must send to the screen
    const uint8_t pixelSize = target.getPixelSize();
    const size_t totalBufferSize = size.x * size.y * pixelSize;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    // Keep track of the current position
    size_t actualPixel = 0;
    Graphics::Vec2D actualPosition = position;
    Graphics::Vec2D radiusPosition {static_cast<int16_t>(-radius), static_cast<int16_t>(-radius)};

    // How many times we reset the Y axis
    unsigned verticalLoopCount = 0;

    // Prepare a large buffer of pixels for faster transmission
    auto *buffer = (uint8_t *)malloc(BUFFER_SIZE);

    target.prepareDrawBuffer();

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Calculate the number of pixels to feed to the screen and the next cursor position
        const size_t pixelsToFeed = (actualStep == numberOfStep - 1) ? (size.x * size.y) % BUFFER_SIZE : BUFFER_SIZE / pixelSize;
        size_t bufferPos = 0;

        for (size_t i = 0; i < pixelsToFeed; ++i)
        {
            if (static_cast<uint16_t>(radiusPosition.x * radiusPosition.x) + static_cast<uint16_t>(radiusPosition.y * radiusPosition.y) <= squareRadius)
                bufferPos = target.putPixelInBuffer(buffer, rawCircleColor, bufferPos);
            else
                bufferPos = target.putPixelInBuffer(buffer, rawBackgroundColor, bufferPos);

            if (++radiusPosition.x == radius)
            {
                radiusPosition.x = -radius;
                ++radiusPosition.y;
            }
        }

        // Draw the buffer
        const bool mustContinue = target.drawBuffer(position, size, actualPixel, actualPosition, buffer,
                                                    pixelsToFeed, verticalLoopCount, loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);
}

uint16_t Graphics::GfxUtils::drawChar(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position, char c,
                                      const FONT_INFO &fontInfo, const Graphics::Color &textColor,
                                      const Graphics::Color &backgroundColor, bool loopVerticalAxis)
{
    // Check if the char is supported
    if (c < fontInfo.startChar || c > fontInfo.endChar)
        c = '?';

    // Set the window
    const size_t descriptorOffset = c - fontInfo.startChar;
    const FONT_CHAR_INFO descriptor = fontInfo.charInfo[descriptorOffset];
    Graphics::Vec2D glyphSize = {descriptor.widthBits, fontInfo.height};

    const Graphics::Vec2D framebufferSize = target.getFramebufferSize();
    const uint8_t pixelSize = target.getPixelSize();

    // Check if the character will be seen on the X axis
    if (position.x < -descriptor.widthBits || position.x > framebufferSize.x)
        return descriptor.widthBits;

    target.setWindow(position, glyphSize);

    // Get raw color
    uint32_t rawTextColor = target.convertColorToRaw(textColor);
    uint32_t rawBackgroundColor = target.convertColorToRaw(backgroundColor);

    // Prepare the Screen controller to receive data
    target.prepareDrawBuffer();

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
                    bufferPos = target.putPixelInBuffer(buffer, rawTextColor, bufferPos);
                else
                    bufferPos = target.putPixelInBuffer(buffer, rawBackgroundColor, bufferPos);

                ++actualPixel;
                ++actualPixelInGlyph;
            }
            else
            {
                actualPixelInGlyph += overflowX;
            }
        }

        // Draw the buffer
        const bool mustContinue = target.drawBuffer(position, glyphSize, actualPixel, actualPosition, buffer,
                                             bufferPos / pixelSize, verticalLoopCount, loopVerticalAxis);

        if (!mustContinue)
            break;
    }

    // Free the buffer
    free(buffer);

    // Return the char width
    return descriptor.widthBits;
}

inline size_t Graphics::GfxUtils::positionToPixelNbr(const Graphics::Vec2D &position, const Graphics::Vec2D &basePosition,
                                              const Graphics::Vec2D &size)
{
    const Graphics::Vec2D normPosition = position - basePosition;
    return normPosition.y * size.x + normPosition.x;
}

inline void Graphics::GfxUtils::fillBufferWithColor(const Hardware::Screen::BaseScreen &target, uint8_t *buffer,
                                                const size_t size, const uint32_t rawColor)
{
    size_t i = 0;
    while ((i = target.putPixelInBuffer(buffer, rawColor, i)) < size) {}
}
