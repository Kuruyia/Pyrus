#ifndef PYRUS_ST7789_H
#define PYRUS_ST7789_H

#include <string>

#include <nrf_font.h>

#include "Graphics/Color.h"
#include "Graphics/Vec2D.h"

namespace Hardware
{

namespace LCD
{

class ST7789 {
public:
    ST7789(uint8_t width, uint8_t height, uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs, uint8_t cd, uint8_t reset);

    void setWindow(const Vec2D_t &position, const Vec2D_t &size);

    void drawPixel(const Vec2D_t &position, Color565_t color);
    void drawRectangle(const Vec2D_t &position, const Vec2D_t &size, Color565_t color);

    uint16_t drawChar(const Vec2D_t &position, const char c, const FONT_INFO &fontInfo, const Color565_t &textColor,
                      const Color565_t &backgroundColor);
    void drawString(Vec2D_t position, const std::string &text, const FONT_INFO &fontInfo,
                    const Color565_t &textColor, const Color565_t &backgroundColor);

private:
    void setCommandPin();
    void setDataPin();

    void init();

    const uint8_t m_width;
    const uint8_t m_height;

    nrfx_spim_config_t m_lcdSpiConfig{};
    const uint8_t m_mosi;
    const uint8_t m_miso;
    const uint8_t m_clk;
    const uint8_t m_cs;
    const uint8_t m_cd;
    const uint8_t m_reset;
};

} // namespace LCD

} // namespace Hardware


#endif //PYRUS_ST7789_H
