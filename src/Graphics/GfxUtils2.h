#ifndef PYRUS_GFXUTILS2_H
#define PYRUS_GFXUTILS2_H

#include "../Hardware/Screen/BaseScreen.h"

namespace Graphics
{

class GfxUtils2 {
public:
    GfxUtils2(Hardware::Screen::BaseScreen &target, const Graphics::Color &fillColor = {255, 255, 255},
            const Graphics::Color &backgroundColor = {0, 0, 0});

    const Color &getFillColor() const;
    void setFillColor(const Color &fillColor);

    const Color &getBackgroundColor() const;
    void setBackgroundColor(const Color &backgroundColor);

    bool isLoopVerticalAxis() const;
    void setLoopVerticalAxis(bool loopVerticalAxis);

    bool isClippingEnabled() const;
    void setClippingEnabled(bool clippingEnabled);

    const Vec2D &getClippingStart() const;
    void setClippingStart(const Vec2D &clippingStart);

    const Vec2D &getClippingEnd() const;
    void setClippingEnd(const Vec2D &clippingEnd);

    void drawChar(Graphics::Vec2D position, char c, const FONT_INFO &fontInfo);
    static void getCharGeometry(Graphics::Vec2D &geometry, char c, const FONT_INFO &fontInfo);

    void drawLine(Graphics::Vec2D firstPoint, Graphics::Vec2D secondPoint);

private:
    void getDrawingWindow(Graphics::Vec2D &windowStart, Graphics::Vec2D &windowEnd);

    Hardware::Screen::BaseScreen &m_target;

    Graphics::Color m_fillColor;
    Graphics::Color m_backgroundColor;

    bool m_loopVerticalAxis;

    bool m_clippingEnabled;
    Graphics::Vec2D m_clippingStart;
    Graphics::Vec2D m_clippingEnd;
}; // class GfxUtils2

} // namespace Graphics

#endif //PYRUS_GFXUTILS2_H
