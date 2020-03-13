#ifndef PYRUS_ST7789_H
#define PYRUS_ST7789_H

#include <cstdint>

namespace Hardware
{

namespace LCD
{

class ST7789 {
public:
    struct color_t
    {
        uint8_t r : 5;
        uint8_t g : 6;
        uint8_t b : 5;
    };

    ST7789(uint8_t width, uint8_t height, uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs, uint8_t cd, uint8_t reset);

    void setWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    void drawPixel(uint16_t x, uint16_t y, color_t color);
    void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, color_t color);

    void drawChar(uint16_t x, uint16_t y, char c, color_t color);

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
