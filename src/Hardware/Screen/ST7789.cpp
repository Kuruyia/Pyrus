#include <cstring>
#include <cstdlib>

#include <drivers/include/nrfx_spim.h>
#include <hal/nrf_gpio.h>
#include <libraries/delay/nrf_delay.h>

#include "Graphics/GfxUtils.h"

#include "ST7789.h"

#define ST7789_CMD_SWRESET 0x01
#define ST7789_CMD_SLPOUT  0x11
#define ST7789_CMD_NORON   0x13
#define ST7789_CMD_INVON   0x21
#define ST7789_CMD_DISPON  0x29
#define ST7789_CMD_CASET   0x2A
#define ST7789_CMD_RASET   0x2B
#define ST7789_CMD_RAMWR   0x2C
#define ST7789_CMD_VSCRDEF 0x33
#define ST7789_CMD_COLMOD  0x3A
#define ST7789_CMD_MADCTL  0x36
#define ST7789_CMD_VSCSAD  0x37

#define BUFFER_SIZE 254

#define FRAMEBUFFER_WIDTH 240
#define FRAMEBUFFER_HEIGHT 320

static const nrfx_spim_t lcdSpi = NRFX_SPIM_INSTANCE(0);

Hardware::Screen::ST7789::ST7789(const Graphics::Vec2D &screenSize, const uint8_t mosi, const uint8_t miso, const uint8_t clk,
                                 const uint8_t cs, const uint8_t cd, const uint8_t reset)
                              : m_screenSize(screenSize)
                              , m_windowPosition({0, 0})
                              , m_windowSize(screenSize)
                              , m_verticalScrollOffset(0)
                              , m_topFixedArea(0)
                              , m_lcdSpiConfig(NRFX_SPIM_DEFAULT_CONFIG)
                              , m_mosi(mosi)
                              , m_miso(miso)
                              , m_clk(clk)
                              , m_cs(cs)
                              , m_cd(cd)
                              , m_reset(reset)
{
    // Configure the pins
    nrf_gpio_cfg_output(m_cd);
    nrf_gpio_cfg_output(m_reset);

    // Configure the SPI master driver
    m_lcdSpiConfig.frequency      = NRF_SPIM_FREQ_8M;
    m_lcdSpiConfig.mosi_pin       = m_mosi;
    m_lcdSpiConfig.miso_pin       = m_miso;
    m_lcdSpiConfig.sck_pin        = m_clk;
    m_lcdSpiConfig.ss_pin         = m_cs;
    m_lcdSpiConfig.ss_active_high = false;
    APP_ERROR_CHECK(nrfx_spim_init(&lcdSpi, &m_lcdSpiConfig, NULL, NULL));

    // Initialize the hardware
    init();
}

void Hardware::Screen::ST7789::init()
{
    // Hardware-reset the Screen controller
    nrf_gpio_pin_clear(m_reset);
    nrf_delay_ms(15);

    nrf_gpio_pin_set(m_reset);
    nrf_delay_ms(2);

    // Prepare the commands to software-reset the controller
    static uint8_t lcdTxSwreset = ST7789_CMD_SWRESET;
    static uint8_t lcdTxSlpout  = ST7789_CMD_SLPOUT;
    nrfx_spim_xfer_desc_t lcdXferSwreset = NRFX_SPIM_XFER_TX(&lcdTxSwreset, 1);
    nrfx_spim_xfer_desc_t lcdXferSlpout  = NRFX_SPIM_XFER_TX(&lcdTxSlpout, 1);

    static uint8_t lcdTxColmodCmd  = ST7789_CMD_COLMOD;
    static uint8_t lcdTxColmodData = 0x55;
    nrfx_spim_xfer_desc_t lcdXferColmodCmd  = NRFX_SPIM_XFER_TX(&lcdTxColmodCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferColmodData = NRFX_SPIM_XFER_TX(&lcdTxColmodData, 1);

    static uint8_t lcdTxMadctlCmd  = ST7789_CMD_MADCTL;
    static uint8_t lcdTxMadctlData = 0;
    nrfx_spim_xfer_desc_t lcdXferMadctlCmd  = NRFX_SPIM_XFER_TX(&lcdTxMadctlCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferMadctlData = NRFX_SPIM_XFER_TX(&lcdTxMadctlData, 1);

    static uint8_t lcdTxCasetCmd = ST7789_CMD_CASET;
    static uint8_t lcdTxCasetData[] = {
            0,
            0,
            static_cast<uint8_t>(m_screenSize.x >> 8),
            static_cast<uint8_t>(m_screenSize.x & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferCasetCmd = NRFX_SPIM_XFER_TX(&lcdTxCasetCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferCasetData = NRFX_SPIM_XFER_TX(lcdTxCasetData, sizeof(lcdTxCasetData));

    static uint8_t lcdTxRasetCmd = ST7789_CMD_RASET;
    static uint8_t lcdTxRasetData[] = {
            0,
            0,
            static_cast<uint8_t>(m_screenSize.y >> 8),
            static_cast<uint8_t>(m_screenSize.y & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferRasetCmd = NRFX_SPIM_XFER_TX(&lcdTxRasetCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferRasetData = NRFX_SPIM_XFER_TX(lcdTxRasetData, sizeof(lcdTxRasetData));

    static uint8_t lcdTxLast[] = {
            ST7789_CMD_INVON,
            ST7789_CMD_NORON,
            ST7789_CMD_DISPON
    };
    nrfx_spim_xfer_desc_t lcdXferLast = NRFX_SPIM_XFER_TX(lcdTxLast, sizeof(lcdTxLast));

    // Send the commands
    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferSwreset, 0));
    nrf_delay_ms(150);

    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferSlpout, 0));
    nrf_delay_ms(10);

    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferColmodCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferColmodData, 0));
    nrf_delay_ms(10);

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferMadctlCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferMadctlData, 0));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferCasetCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferCasetData, 0));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRasetCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRasetData, 0));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferLast, 0));
    nrf_delay_ms(10);
}

void Hardware::Screen::ST7789::sendCommand(const uint8_t command, const uint8_t *data, const size_t dataSize)
{
    nrfx_spim_xfer_desc_t xferCmd  = NRFX_SPIM_XFER_TX(&command, 1);
    nrfx_spim_xfer_desc_t xferData = NRFX_SPIM_XFER_TX(data, dataSize);

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &xferCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &xferData, 0));
}


inline void Hardware::Screen::ST7789::setCommandPin()
{
    nrf_gpio_pin_clear(m_cd);
}

inline void Hardware::Screen::ST7789::setDataPin()
{
    nrf_gpio_pin_set(m_cd);
}

void Hardware::Screen::ST7789::setWindow(const Graphics::Vec2D &position, const Graphics::Vec2D &size)
{
    // Store the window parameters
    m_windowPosition = position;
    m_windowSize = size;

    // Calculate end coordinates
    const uint16_t endX = position.x + size.x - 1;
    const uint16_t endY = position.y + size.y - 1;

    // Set the column address
    uint8_t lcdTxCasetData[] = {
            static_cast<uint8_t>(position.x >> 8),
            static_cast<uint8_t>(position.x & 0xFF),
            static_cast<uint8_t>(endX >> 8),
            static_cast<uint8_t>(endX & 0xFF)
    };

    sendCommand(ST7789_CMD_CASET, lcdTxCasetData, sizeof(lcdTxCasetData));

    // Set the row address
    uint8_t lcdTxRasetData[] = {
            static_cast<uint8_t>(position.y >> 8),
            static_cast<uint8_t>(position.y & 0xFF),
            static_cast<uint8_t>(endY >> 8),
            static_cast<uint8_t>(endY & 0xFF)
    };

    sendCommand(ST7789_CMD_RASET, lcdTxRasetData, sizeof(lcdTxRasetData));
}

void Hardware::Screen::ST7789::getWindow(Graphics::Vec2D &position, Graphics::Vec2D &size) const
{
    position = m_windowPosition;
    size = m_windowSize;
}

Graphics::Vec2D Hardware::Screen::ST7789::getFramebufferSize() const
{
    return {FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT};
}

const Graphics::Vec2D &Hardware::Screen::ST7789::getScreenSize() const
{
    return m_screenSize;
}

void Hardware::Screen::ST7789::setVerticalScrollOffset(uint16_t offset)
{
    // Store the offset
    m_verticalScrollOffset = offset;
    m_verticalScrollOffset %= FRAMEBUFFER_HEIGHT;

    // Set the vertical scroll start address
    uint8_t lcdTxVscsadData[] = {
            static_cast<uint8_t>(m_verticalScrollOffset >> 8),
            static_cast<uint8_t>(m_verticalScrollOffset & 0xFF)
    };

    sendCommand(ST7789_CMD_VSCSAD, lcdTxVscsadData, sizeof(lcdTxVscsadData));
}

const uint16_t &Hardware::Screen::ST7789::getVerticalScrollOffset() const
{
    return m_verticalScrollOffset;
}

void Hardware::Screen::ST7789::setTopFixedArea(uint16_t area)
{
    // Store the area
    m_topFixedArea = area;

    // Calculate the scrolling area
    const uint16_t scrollingArea = FRAMEBUFFER_HEIGHT - area;

    // Set the vertical scroll start address
    uint8_t lcdTxVscrdefData[] = {
            static_cast<uint8_t>(area >> 8),
            static_cast<uint8_t>(area & 0xFF),
            static_cast<uint8_t>(scrollingArea >> 8),
            static_cast<uint8_t>(scrollingArea & 0xFF),
            0,
            0
    };

    sendCommand(ST7789_CMD_VSCRDEF, lcdTxVscrdefData, sizeof(lcdTxVscrdefData));
}

const uint16_t &Hardware::Screen::ST7789::getTopFixedArea() const
{
    return m_topFixedArea;
}

void Hardware::Screen::ST7789::clearFramebuffer(const Graphics::Color &color)
{
    // Draw a rectangle taking the entire framebuffer
    Graphics::GfxUtils::drawFilledRectangle(*this, {0, 0}, getFramebufferSize(), color, false);
}

void Hardware::Screen::ST7789::drawPixel(const Graphics::Vec2D &position, const Graphics::Color &color)
{
    // Set the window
    setWindow(position, {1, 1});

    // Get raw color
    const Graphics::Color color565 = color.convertToColorEncoding<5, 6, 5>();
    uint16_t rawColor = (color565.getRed() << 11) | (color565.getGreen() << 5) | color565.getBlue();

    // Send the color to draw
    uint8_t lcdTxRamwrData[] = {
            static_cast<uint8_t>(rawColor >> 8),
            static_cast<uint8_t>(rawColor & 0xFF)
    };

    sendCommand(ST7789_CMD_RAMWR, lcdTxRamwrData, sizeof(lcdTxRamwrData));
}

void Hardware::Screen::ST7789::prepareDrawBuffer()
{
    setCommandPin();
    static uint8_t lcdTxRamwrCmd  = ST7789_CMD_RAMWR;
    nrfx_spim_xfer_desc_t lcdXferRamwrCmd  = NRFX_SPIM_XFER_TX(&lcdTxRamwrCmd, 1);
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrCmd, 0));

    setDataPin();
}

bool Hardware::Screen::ST7789::drawBuffer(const Graphics::Vec2D &position, const Graphics::Vec2D &size, const size_t &actualPixel,
                                          Graphics::Vec2D &actualPosition, uint8_t *buffer, size_t pixelsToFeed, unsigned &verticalLoopCount,
                                          const bool loopVerticalAxis)
{
    // Each pixel is 2 bytes in the buffer
    pixelsToFeed *= 2;

    // Calculate the new position
    Graphics::Vec2D nextPosition = {static_cast<int16_t>(actualPixel % size.x + position.x),
                          static_cast<int16_t>(actualPixel / size.x + position.y - verticalLoopCount * FRAMEBUFFER_HEIGHT)};

    // Check if we are going to overflow on the Y axis
    if (nextPosition.y >= FRAMEBUFFER_HEIGHT)
    {
        // Calculate the number of pixel before the overflow
        const uint16_t pixelBeforeOverflowCount = (size.x - actualPosition.x + position.x) + ((FRAMEBUFFER_HEIGHT - actualPosition.y - 1) * size.x);
        const uint16_t bufferBeforeOverflowCount = pixelBeforeOverflowCount * 2;

        // Transfer those pixels to the screen
        nrfx_spim_xfer_desc_t lcdXferRamwrData = NRFX_SPIM_XFER_TX(buffer, bufferBeforeOverflowCount);
        APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));

        if (loopVerticalAxis)
        {
            // Reset the window to the beginning of the framebuffer
            setWindow({position.x, 0},
                      {size.x, static_cast<int16_t>(size.y - actualPosition.y)});

            // Transfer the rest of the pixels
            setCommandPin();
            static uint8_t lcdTxRamwrCmd  = ST7789_CMD_RAMWR;
            nrfx_spim_xfer_desc_t lcdXferRamwrCmd  = NRFX_SPIM_XFER_TX(&lcdTxRamwrCmd, 1);
            APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrCmd, 0));

            setDataPin();
            lcdXferRamwrData = NRFX_SPIM_XFER_TX(buffer + bufferBeforeOverflowCount,
                                                 pixelsToFeed - bufferBeforeOverflowCount);
            APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));

            // Update the actual position
            ++verticalLoopCount;
            nextPosition.y -= FRAMEBUFFER_HEIGHT;
            actualPosition = nextPosition;
        }
        else
        {
            // We are asked not to continue
            return false;
        }
    }
    else
    {
        // Send the pixels to the screen controller
        nrfx_spim_xfer_desc_t lcdXferRamwrData = NRFX_SPIM_XFER_TX(buffer, pixelsToFeed);
        APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));

        // Update the actual position
        actualPosition = nextPosition;
    }

    return true;
}

uint32_t Hardware::Screen::ST7789::convertColorToRaw(const Graphics::Color &color) const
{
    Graphics::Color color565 = color.convertToColorEncoding<5, 6, 5>();
    return (color565.getRed() << 11) | (color565.getGreen() << 5) | color565.getBlue();
}

size_t Hardware::Screen::ST7789::putPixelInBuffer(uint8_t *buffer, uint32_t rawColor, size_t pos) const
{
    buffer[pos] = static_cast<uint8_t>(rawColor >> 8);
    buffer[pos + 1] = static_cast<uint8_t>(rawColor & 0xFF);

    return pos + 2;
}

uint8_t Hardware::Screen::ST7789::getPixelSize() const
{
    return 2;
}