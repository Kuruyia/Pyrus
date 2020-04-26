#ifndef PYRUS_GFXUTILS_H
#define PYRUS_GFXUTILS_H

#include "../Hardware/Screen/BaseScreen.h"

#include "Color.h"
#include "Vec2D.h"

namespace Graphics {

class GfxUtils {
public:
    virtual ~GfxUtils() = 0;

    static void drawLine(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint, Graphics::Vec2D secondPoint,
            const Graphics::Color &color, bool loopVerticalAxis = true);

    static void drawFastLine(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint, Graphics::Vec2D secondPoint,
            const Graphics::Color &lineColor, const Graphics::Color &backgroundColor = {0, 0, 0},
            bool loopVerticalAxis = true);

    static void drawFilledRectangle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
            const Graphics::Vec2D &size, const Graphics::Color &color, bool loopVerticalAxis = true);

    static void drawFilledCircle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &origin,
            uint16_t radius, const Graphics::Color &circleColor, const Graphics::Color &backgroundColor = {0, 0, 0},
            bool loopVerticalAxis = true);

    static uint16_t drawChar(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position, char c,
            const FONT_INFO &fontInfo, const Graphics::Color &textColor, const Graphics::Color &backgroundColor = {0, 0, 0},
            bool loopVerticalAxis = true);

private:
    static size_t positionToPixelNbr(const Graphics::Vec2D &position, const Graphics::Vec2D &basePosition,
                                     const Graphics::Vec2D &size);

    static void fillBufferWithColor(const Hardware::Screen::BaseScreen &target, uint8_t *buffer, size_t size,
                                    uint32_t rawColor);
}; // class GfxUtils

} // namespace Graphics

#endif //PYRUS_GFXUTILS_H
