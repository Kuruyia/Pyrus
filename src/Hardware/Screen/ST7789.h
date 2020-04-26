#ifndef PYRUS_ST7789_H
#define PYRUS_ST7789_H

#include <string>

#include "nrfx_spim.h"

#include "BaseScreen.h"

namespace Hardware
{

namespace Screen
{

class ST7789 : public BaseScreen {
public:
    ST7789(const Graphics::Vec2D &screenSize, uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t cs, uint8_t cd, uint8_t reset);
    ~ST7789() override = default;

    void setWindow(const Graphics::Vec2D &position, const Graphics::Vec2D &size) override;
    void getWindow(Graphics::Vec2D &position, Graphics::Vec2D &size) const override;

    const Graphics::Vec2D &getScreenSize() const override;
    Graphics::Vec2D getFramebufferSize() const override;

    void setVerticalScrollOffset(uint16_t offset) override;
    const uint16_t &getVerticalScrollOffset() const override;

    void setTopFixedArea(uint16_t area) override;
    const uint16_t &getTopFixedArea() const override;

    void clearFramebuffer(const Graphics::Color &color) override;

    uint32_t convertColorToRaw(const Graphics::Color &color) const override;
    size_t putPixelInBuffer(uint8_t *buffer, uint32_t rawColor, size_t pos) const override;
    uint8_t getPixelSize() const override;

    void prepareDrawBuffer() override;
    bool drawBuffer(const Graphics::Vec2D &position, const Graphics::Vec2D &size, const size_t &actualPixel,
                    Graphics::Vec2D &actualPosition, uint8_t *buffer, size_t pixelsToFeed, unsigned &verticalLoopCount,
                    bool loopVerticalAxis) override;

    void drawPixel(const Graphics::Vec2D &position, const Graphics::Color &color) override;

private:
    void setCommandPin();
    void setDataPin();

    void init();

    void sendCommand(uint8_t command, const uint8_t *data, size_t dataSize);

    const Graphics::Vec2D m_screenSize;

    Graphics::Vec2D m_windowPosition;
    Graphics::Vec2D m_windowSize;

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
