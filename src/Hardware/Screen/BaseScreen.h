#ifndef PYRUS_BASESCREEN_H
#define PYRUS_BASESCREEN_H

#include <cstddef>
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

    virtual void setWindow(const Graphics::Vec2D &position, const Graphics::Vec2D &size) = 0;
    virtual void getWindow(Graphics::Vec2D &position, Graphics::Vec2D &size) const = 0;

    virtual const Graphics::Vec2D &getScreenSize() const = 0;
    virtual Graphics::Vec2D getFramebufferSize() const = 0;

    virtual void setVerticalScrollOffset(uint16_t offset) = 0;
    virtual const uint16_t &getVerticalScrollOffset() const = 0;

    virtual void setTopFixedArea(uint16_t area) = 0;
    virtual const uint16_t &getTopFixedArea() const = 0;

    virtual void clearFramebuffer(const Graphics::Color &color) = 0;

    virtual bool drawBuffer(const Graphics::Vec2D &position, const Graphics::Vec2D &size, const size_t &actualPixel,
                            Graphics::Vec2D &actualPosition, uint8_t *buffer, size_t pixelsToFeed, unsigned &verticalLoopCount,
                            bool loopVerticalAxis) = 0;

    virtual void drawPixel(const Graphics::Vec2D &position, const Graphics::Color &color) = 0;
    virtual void drawRectangle(const Graphics::Vec2D &position, const Graphics::Vec2D &size, const Graphics::Color &color,
                               bool loopVerticalAxis) = 0;

    virtual uint16_t drawChar(const Graphics::Vec2D &position, char c, const FONT_INFO &fontInfo, const Graphics::Color &textColor,
                              const Graphics::Color &backgroundColor, bool loopVerticalAxis) = 0;
}; // class BaseScreen

} // namespace Screen

} // namespace Hardware

#endif //PYRUS_BASESCREEN_H
