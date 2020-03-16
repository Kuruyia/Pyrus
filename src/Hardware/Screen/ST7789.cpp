#include <cstring>
#include <cstdlib>

#include <drivers/include/nrfx_spim.h>
#include <hal/nrf_gpio.h>
#include <libraries/delay/nrf_delay.h>

#include "ST7789.h"

#define ST7789_CMD_SWRESET 0x01
#define ST7789_CMD_SLPOUT  0x11
#define ST7789_CMD_NORON   0x13
#define ST7789_CMD_INVON   0x21
#define ST7789_CMD_DISPON  0x29
#define ST7789_CMD_CASET   0x2A
#define ST7789_CMD_RASET   0x2B
#define ST7789_CMD_RAMWR   0x2C
#define ST7789_CMD_COLMOD  0x3A
#define ST7789_CMD_MADCTL  0x36
#define ST7789_CMD_VSCSAD  0x37

#define BUFFER_SIZE 254

#define FRAMEBUFFER_WIDTH 240
#define FRAMEBUFFER_HEIGHT 320

static const nrfx_spim_t lcdSpi = NRFX_SPIM_INSTANCE(0);

Hardware::Screen::ST7789::ST7789(const Vec2D_t &screenSize, const uint8_t mosi, const uint8_t miso, const uint8_t clk,
                                 const uint8_t cs, const uint8_t cd, const uint8_t reset)
                              : m_screenSize(screenSize)
                              , m_verticalScrollOffset(0)
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
    m_lcdSpiConfig = NRFX_SPIM_DEFAULT_CONFIG;
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

inline void Hardware::Screen::ST7789::setCommandPin()
{
    nrf_gpio_pin_clear(m_cd);
}

inline void Hardware::Screen::ST7789::setDataPin()
{
    nrf_gpio_pin_set(m_cd);
}

void Hardware::Screen::ST7789::setWindow(const Vec2D_t &position, const Vec2D_t &size)
{
    // Store the window parameters
    m_windowPosition = position;
    m_windowSize = size;

    // Calculate end coordinates
    const uint16_t endX = position.x + size.x - 1;
    const uint16_t endY = position.y + size.y - 1;

    // Set the column address
    static uint8_t lcdTxCasetCmd  = ST7789_CMD_CASET;
    uint8_t lcdTxCasetData[] = {
            static_cast<uint8_t>(position.x >> 8),
            static_cast<uint8_t>(position.x & 0xFF),
            static_cast<uint8_t>(endX >> 8),
            static_cast<uint8_t>(endX & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferCasetCmd  = NRFX_SPIM_XFER_TX(&lcdTxCasetCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferCasetData = NRFX_SPIM_XFER_TX(lcdTxCasetData, sizeof(lcdTxCasetData));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferCasetCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferCasetData, 0));

    // Set the row address
    static uint8_t lcdTxRasetCmd  = ST7789_CMD_RASET;
    uint8_t lcdTxRasetData[] = {
            static_cast<uint8_t>(position.y >> 8),
            static_cast<uint8_t>(position.y & 0xFF),
            static_cast<uint8_t>(endY >> 8),
            static_cast<uint8_t>(endY & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferRasetCmd  = NRFX_SPIM_XFER_TX(&lcdTxRasetCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferRasetData = NRFX_SPIM_XFER_TX(lcdTxRasetData, sizeof(lcdTxRasetData));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRasetCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRasetData, 0));
}

void Hardware::Screen::ST7789::getWindow(Vec2D_t &position, Vec2D_t &size) const
{
    position = m_windowPosition;
    size = m_windowSize;
}

Vec2D_t Hardware::Screen::ST7789::getFramebufferSize() const
{
    return {FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT};
}

const Vec2D_t &Hardware::Screen::ST7789::getScreenSize() const
{
    return m_screenSize;
}

void Hardware::Screen::ST7789::setVerticalScrollOffset(uint16_t offset)
{
    // Store the offset
    m_verticalScrollOffset = offset;

    // Set the vertical scroll start address
    static uint8_t lcdTxVscsadCmd  = ST7789_CMD_VSCSAD;
    uint8_t lcdTxVscsadData[] = {
            static_cast<uint8_t>(offset >> 8),
            static_cast<uint8_t>(offset & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferVscsadCmd  = NRFX_SPIM_XFER_TX(&lcdTxVscsadCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferVscsadData = NRFX_SPIM_XFER_TX(lcdTxVscsadData, sizeof(lcdTxVscsadData));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferVscsadCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferVscsadData, 0));
}

const uint16_t &Hardware::Screen::ST7789::getVerticalScrollOffset() const
{
    return m_verticalScrollOffset;
}


void Hardware::Screen::ST7789::clearFramebuffer(Color565_t color)
{
    // Draw a rectangle taking the entire framebuffer
    drawRectangle({0, 0}, getFramebufferSize(), color);
}

void Hardware::Screen::ST7789::drawPixel(const Vec2D_t &position, Color565_t color)
{
    // Set the window
    setWindow(position, {1, 1});

    // Get raw color
    uint16_t rawColor = ((color.g & 0x7) << 13) | (color.b << 8) | (color.r << 3) | (color.g >> 3);

    // Send the color to draw
    static uint8_t lcdTxRamwrCmd  = ST7789_CMD_RAMWR;
    uint8_t lcdTxRamwrData[] = {
            static_cast<uint8_t>(rawColor >> 8),
            static_cast<uint8_t>(rawColor & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferRamwrCmd  = NRFX_SPIM_XFER_TX(&lcdTxRamwrCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferRamwrData = NRFX_SPIM_XFER_TX(lcdTxRamwrData, sizeof(lcdTxRamwrData));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));
}

void Hardware::Screen::ST7789::drawRectangle(const Vec2D_t &position, const Vec2D_t &size, Color565_t color)
{
    // Set the window
    setWindow(position, size);

    // Get raw color
    uint16_t rawColor = ((color.g & 0x7) << 13) | (color.b << 8) | (color.r << 3) | (color.g >> 3);

    // Send the color to draw
    static uint8_t lcdTxRamwrCmd  = ST7789_CMD_RAMWR;

    // Prepare a large buffer of pixels for faster transmission
    auto *lcdTxRamwrData = (uint8_t *)malloc(BUFFER_SIZE);
    for (size_t i = 0; i < BUFFER_SIZE; i += sizeof(rawColor))
    {
        memcpy(lcdTxRamwrData + i, &rawColor, sizeof(rawColor));
    }
    nrfx_spim_xfer_desc_t lcdXferRamwrCmd  = NRFX_SPIM_XFER_TX(&lcdTxRamwrCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferRamwrData = NRFX_SPIM_XFER_TX(lcdTxRamwrData, BUFFER_SIZE);

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrCmd, 0));
    setDataPin();

    // Send the pixels
    uint32_t i = 0;
    for (; i < (size.x * size.y) / (BUFFER_SIZE / 2); ++i)
    {
        APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));
    }

    // Send the remaining pixels that were not part of a full BUFFER_SIZE block
    lcdXferRamwrData = NRFX_SPIM_XFER_TX(lcdTxRamwrData, static_cast<size_t>((size.x * size.y) % (BUFFER_SIZE / 2) * 2));
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));

    // Free the buffer
    free(lcdTxRamwrData);
}

uint16_t Hardware::Screen::ST7789::drawChar(const Vec2D_t &position, const char c, const FONT_INFO &fontInfo,
                                            const Color565_t &textColor, const Color565_t &backgroundColor)
{
    // Set the window
    const size_t descriptorOffset = c - fontInfo.startChar;
    const FONT_CHAR_INFO descriptor = fontInfo.charInfo[descriptorOffset];

    setWindow(position, {descriptor.widthBits, fontInfo.height});

    // Get raw color
    const uint16_t rawTextColor = (textColor.r << 11) | (textColor.g << 5) | textColor.b;
    const uint8_t higherTextColor = rawTextColor >> 8;
    const uint8_t lowerTextColor = rawTextColor & 0xFF;

    const uint16_t rawBackgroundColor = (backgroundColor.r << 11) | (backgroundColor.g << 5) | backgroundColor.b;
    const uint8_t higherBackgroundColor = rawBackgroundColor >> 8;
    const uint8_t lowerBackgroundColor = rawBackgroundColor & 0xFF;

    // Prepare the Screen controller to receive data
    static uint8_t lcdTxRamwrCmd  = ST7789_CMD_RAMWR;
    nrfx_spim_xfer_desc_t lcdXferRamwrCmd  = NRFX_SPIM_XFER_TX(&lcdTxRamwrCmd, 1);

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrCmd, 0));
    setDataPin();

    // Prepare some values for character parsing
    const uint16_t pixelCount = descriptor.widthBits * fontInfo.height;
    size_t actualPixel = 0;
    const uint8_t bytesPerWidth = (descriptor.widthBits % 8 > 0) ? descriptor.widthBits / 8 + 1 : descriptor.widthBits / 8;
    const size_t totalBufferSize = pixelCount * 2;
    const size_t numberOfStep = (totalBufferSize % BUFFER_SIZE > 0) ? totalBufferSize / BUFFER_SIZE + 1 : totalBufferSize / BUFFER_SIZE;

    for (size_t actualStep = 0; actualStep < numberOfStep; ++actualStep)
    {
        // Prepare the buffer to send data to the Screen
        const size_t endPixel = (actualStep == numberOfStep - 1) ? pixelCount : (BUFFER_SIZE / 2) * (actualStep + 1);
        const size_t bufferSize = (actualStep == numberOfStep - 1) ? (pixelCount * 2) - (BUFFER_SIZE * actualStep) : BUFFER_SIZE;
        auto *buffer = (uint8_t *)malloc(bufferSize);
        size_t bufferPos = 0;

        // Read data from the font definition
        for (; actualPixel < endPixel; ++actualPixel)
        {
            const uint8_t bitInBlock = actualPixel % descriptor.widthBits;
            const size_t actualByte = (bitInBlock / 8) + (bytesPerWidth * (actualPixel / descriptor.widthBits));
            const uint8_t actualVal = fontInfo.data[descriptor.offset + actualByte] & (1 << (bitInBlock % 8));

            if (actualVal)
            {
                buffer[bufferPos++] = higherTextColor;
                buffer[bufferPos++] = lowerTextColor;
            }
            else
            {
                buffer[bufferPos++] = higherBackgroundColor;
                buffer[bufferPos++] = lowerBackgroundColor;
            }
        }

        // Send the pixels to the Screen controller
        nrfx_spim_xfer_desc_t lcdXferRamwrData = NRFX_SPIM_XFER_TX(buffer, bufferSize);
        APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));

        // Free the buffer
        free(buffer);
    }

    // Return the char width
    return descriptor.widthBits;
}