#ifndef PYRUS_GFXUTILS_H
#define PYRUS_GFXUTILS_H

#include "../Hardware/Screen/BaseScreen.h"

#include "Color.h"
#include "Vec2D.h"

namespace Graphics {

class GfxUtils {
public:
    virtual ~GfxUtils() = 0;

    static void drawFilledRectangle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position,
                                    const Graphics::Vec2D &size, const Graphics::Color &color, bool loopVerticalAxis);

    static void drawFilledCircle(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &origin,
            uint16_t radius, const Graphics::Color &circleColor, const Graphics::Color &backgroundColor,
            bool loopVerticalAxis);

    static uint16_t drawChar(Hardware::Screen::BaseScreen &target, const Graphics::Vec2D &position, char c,
            const FONT_INFO &fontInfo, const Graphics::Color &textColor, const Graphics::Color &backgroundColor,
            bool loopVerticalAxis);
}; // class GfxUtils

} // namespace Graphics

#endif //PYRUS_GFXUTILS_H
