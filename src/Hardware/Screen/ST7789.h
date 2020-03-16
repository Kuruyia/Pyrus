#ifndef PYRUS_ST7789_H
#define PYRUS_ST7789_H

#include <string>

#include "BaseScreen.h"

namespace Hardware
{

namespace Screen
{

class ST7789 : public BaseScreen {
public:
    ST7789(const Vec2D_t &screenSize, uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs, uint8_t cd, uint8_t reset);
    virtual ~ST7789() = default;

    virtual void setWindow(const Vec2D_t &position, const Vec2D_t &size) override;
    virtual void getWindow(Vec2D_t &position, Vec2D_t &size) const override;

    virtual const Vec2D_t &getScreenSize() const override;
    virtual Vec2D_t getFramebufferSize() const override;

    virtual void setVerticalScrollOffset(uint16_t offset) override;
    virtual const uint16_t &getVerticalScrollOffset() const override;

    virtual void clearFramebuffer(Color565_t color) override;

    virtual void drawPixel(const Vec2D_t &position, Color565_t color) override;
    virtual void drawRectangle(const Vec2D_t &position, const Vec2D_t &size, Color565_t color) override;

    virtual uint16_t drawChar(const Vec2D_t &position, const char c, const FONT_INFO &fontInfo, const Color565_t &textColor,
                      const Color565_t &backgroundColor) override;

private:
    void setCommandPin();
    void setDataPin();

    void init();

    const Vec2D_t m_screenSize;

    Vec2D_t m_windowPosition;
    Vec2D_t m_windowSize;

    uint16_t m_verticalScrollOffset;

    nrfx_spim_config_t m_lcdSpiConfig{};
    const uint8_t m_mosi;
    const uint8_t m_miso;
    const uint8_t m_clk;
    const uint8_t m_cs;
    const uint8_t m_cd;
    const uint8_t m_reset;
}; // class ST7789

} // namespace Screen

} // namespace Hardware


#endif //PYRUS_ST7789_H
