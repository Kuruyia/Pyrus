#ifndef PYRUS_BASESCREEN_H
#define PYRUS_BASESCREEN_H

#include <nrf_font.h>

#include "Graphics/Color.h"
#include "Graphics/Vec2D.h"

namespace Hardware
{

namespace Screen
{

class BaseScreen {
public:
    virtual ~BaseScreen() = default;

    virtual void setWindow(const Vec2D_t &position, const Vec2D_t &size) = 0;
    virtual void getWindow(Vec2D_t &position, Vec2D_t &size) const = 0;

    virtual const Vec2D_t &getScreenSize() const = 0;
    virtual Vec2D_t getFramebufferSize() const = 0;

    virtual void setVerticalScrollOffset(uint16_t offset) = 0;
    virtual const uint16_t &getVerticalScrollOffset() const = 0;

    virtual void clearFramebuffer(Color565_t color) = 0;

    virtual void drawPixel(const Vec2D_t &position, Color565_t color) = 0;
    virtual void drawRectangle(const Vec2D_t &position, const Vec2D_t &size, Color565_t color,
                               bool loopVerticalAxis) = 0;

    virtual uint16_t drawChar(const Vec2D_t &position, char c, const FONT_INFO &fontInfo, const Color565_t &textColor,
                              const Color565_t &backgroundColor, bool loopVerticalAxis) = 0;
}; // class BaseScreen

} // namespace Screen

} // namespace Hardware

#endif //PYRUS_BASESCREEN_H
