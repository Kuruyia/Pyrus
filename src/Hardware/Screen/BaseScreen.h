#ifndef PYRUS_BASESCREEN_H
#define PYRUS_BASESCREEN_H

#include "Graphics/Color.h"
#include <Graphics/Vec2D.h>

namespace Hardware
{

namespace Screen
{

class BaseScreen {
public:
    virtual ~BaseScreen() = default;

    virtual void setWindow(const Vec2D_t &position, const Vec2D_t &size) = 0;
    virtual void getWindow(Vec2D_t &position, Vec2D_t &size) const = 0;

    virtual void drawPixel(const Vec2D_t &position, Color565_t color) = 0;
    virtual void drawRectangle(const Vec2D_t &position, const Vec2D_t &size, Color565_t color) = 0;

    virtual uint16_t drawChar(const Vec2D_t &position, char c, const FONT_INFO &fontInfo, const Color565_t &textColor,
                      const Color565_t &backgroundColor) = 0;
    virtual void drawString(Vec2D_t position, const std::string &text, const FONT_INFO &fontInfo,
                    const Color565_t &textColor, const Color565_t &backgroundColor) = 0;
};

}

}

#endif //PYRUS_BASESCREEN_H
