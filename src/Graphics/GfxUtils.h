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
            const Graphics::Color &color, bool loopVerticalAxis = false);

    static void drawHorizontalLine(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position, uint16_t width,
            const Graphics::Color &color, bool loopVerticalAxis = false);

    static void drawVerticalLine(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position, uint16_t height,
            const Graphics::Color &color, bool loopVerticalAxis = false);

    static void drawFilledRectangle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
            const Graphics::Vec2D &size, const Graphics::Color &color, bool loopVerticalAxis = false);

    static void drawTriangle(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
                             Graphics::Vec2D secondPoint, Graphics::Vec2D thirdPoint, const Graphics::Color &color,
                             bool loopVerticalAxis = false);

    static void drawFilledTriangle(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
            Graphics::Vec2D secondPoint, Graphics::Vec2D thirdPoint, const Graphics::Color &triangleColor,
            const Graphics::Color &backgroundColor = {0, 0, 0}, bool loopVerticalAxis = false);

    static void drawFastFilledTriangle(Hardware::Screen::BaseScreen &target, Graphics::Vec2D firstPoint,
            Graphics::Vec2D secondPoint, Graphics::Vec2D thirdPoint, const Graphics::Color &triangleColor,
            const Graphics::Color &backgroundColor = {0, 0, 0}, bool loopVerticalAxis = false);

    static void drawFilledCircle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &origin,
            uint16_t radius, const Graphics::Color &circleColor, const Graphics::Color &backgroundColor = {0, 0, 0},
            bool loopVerticalAxis = false);

    static uint16_t drawChar(Hardware::Screen::BaseScreen &target, Graphics::Vec2D position, char c,
            const FONT_INFO &fontInfo, const Graphics::Color &textColor, const Graphics::Color &backgroundColor = {0, 0, 0},
            bool loopVerticalAxis = false);

    static void getCharGeometry(Graphics::Vec2D &geometry, char c, const FONT_INFO &fontInfo);

private:
    static size_t positionToPixelNbr(const Graphics::Vec2D &position, const Graphics::Vec2D &basePosition,
                                     const Graphics::Vec2D &size);

    static void fillBufferWithColor(const Hardware::Screen::BaseScreen &target, uint8_t *buffer, size_t size,
                                    uint32_t rawColor);
}; // class GfxUtils

} // namespace Graphics

#endif //PYRUS_GFXUTILS_H
