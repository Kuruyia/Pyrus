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
    ~ST7789() override = default;

    void setWindow(const Vec2D_t &position, const Vec2D_t &size) override;
    void getWindow(Vec2D_t &position, Vec2D_t &size) const override;

    const Vec2D_t &getScreenSize() const override;
    Vec2D_t getFramebufferSize() const override;

    void setVerticalScrollOffset(uint16_t offset) override;
    const uint16_t &getVerticalScrollOffset() const override;

    void setTopFixedArea(uint16_t area) override;
    const uint16_t &getTopFixedArea() const override;

    void clearFramebuffer(Color565_t color) override;

    bool drawBuffer(const Vec2D_t &position, const Vec2D_t &size, const size_t &actualPixel,
                    Vec2D_t &actualPosition, const uint8_t *buffer, size_t pixelsToFeed, unsigned &verticalLoopCount,
                    bool loopVerticalAxis) override;

    void drawPixel(const Vec2D_t &position, Color565_t color) override;
    void drawRectangle(const Vec2D_t &position, const Vec2D_t &size, Color565_t color,
                       bool loopVerticalAxis) override;

    uint16_t drawChar(const Vec2D_t &position, char c, const FONT_INFO &fontInfo, const Color565_t &textColor,
                      const Color565_t &backgroundColor, bool loopVerticalAxis) override;

private:
    void setCommandPin();
    void setDataPin();

    void init();

    void sendCommand(uint8_t command, const uint8_t *data, size_t dataSize);

    const Vec2D_t m_screenSize;

    Vec2D_t m_windowPosition;
    Vec2D_t m_windowSize;

    uint16_t m_verticalScrollOffset;
    uint16_t m_topFixedArea;

    nrfx_spim_config_t m_lcdSpiConfig;
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
