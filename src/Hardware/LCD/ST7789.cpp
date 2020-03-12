#include <drivers/include/nrfx_spim.h>
#include <hal/nrf_gpio.h>
#include <libraries/delay/nrf_delay.h>
#include <libraries/log/nrf_log.h>

#include "ST7789.h"

#define ST7789_CMD_SWRESET 0x01
#define ST7789_CMD_SLPOUT  0x11
#define ST7789_CMD_COLMOD  0x3A
#define ST7789_CMD_MADCTL  0x36
#define ST7789_CMD_CASET   0x2A
#define ST7789_CMD_RASET   0x2B
#define ST7789_CMD_INVON   0x21
#define ST7789_CMD_NORON   0x13
#define ST7789_CMD_DISPON  0x29
#define ST7789_CMD_RAMWR   0x2C

static const nrfx_spim_t lcdSpi = NRFX_SPIM_INSTANCE(0);

Hardware::LCD::ST7789::ST7789(uint8_t width, uint8_t height, const uint8_t mosi, const uint8_t miso, const uint8_t clk,
                              const uint8_t cs, const uint8_t cd, const uint8_t reset)
                              : m_width(width)
                              , m_height(height)
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

    NRF_LOG_INFO("ST7789 init!\n");
    nrf_gpio_pin_set(20);
}

void Hardware::LCD::ST7789::init()
{
    // Hardware-reset the LCD controller
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
            static_cast<uint8_t>(m_width >> 8),
            static_cast<uint8_t>(m_width & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferCasetCmd = NRFX_SPIM_XFER_TX(&lcdTxCasetCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferCasetData = NRFX_SPIM_XFER_TX(lcdTxCasetData, sizeof(lcdTxCasetData));

    static uint8_t lcdTxRasetCmd = ST7789_CMD_RASET;
    static uint8_t lcdTxRasetData[] = {
            0,
            0,
            static_cast<uint8_t>(m_height >> 8),
            static_cast<uint8_t>(m_height & 0xFF)
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

inline void Hardware::LCD::ST7789::setCommandPin()
{
    nrf_gpio_pin_clear(m_cd);
}

inline void Hardware::LCD::ST7789::setDataPin()
{
    nrf_gpio_pin_set(m_cd);
}

void Hardware::LCD::ST7789::setWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    const uint16_t endX = x + width;
    const uint16_t endY = y + height;

    // Set the column address
    static uint8_t lcdTxCasetCmd  = ST7789_CMD_CASET;
    uint8_t lcdTxCasetData[] = {
            static_cast<uint8_t>(x >> 8),
            static_cast<uint8_t>(x & 0xFF),
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
            static_cast<uint8_t>(y >> 8),
            static_cast<uint8_t>(y & 0xFF),
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

void Hardware::LCD::ST7789::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    // Set the window
    setWindow(x, y, 1, 1);

    // Send the color to draw
    static uint8_t lcdTxRamwrCmd  = ST7789_CMD_RAMWR;
    uint8_t lcdTxRamwrData[] = {
            static_cast<uint8_t>(color >> 8),
            static_cast<uint8_t>(color & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferRamwrCmd  = NRFX_SPIM_XFER_TX(&lcdTxRamwrCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferRamwrData = NRFX_SPIM_XFER_TX(lcdTxRamwrData, sizeof(lcdTxRamwrData));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrCmd, 0));
    setDataPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));
}

void Hardware::LCD::ST7789::drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    // Set the window
    setWindow(x, y, width, height);

    // Send the color to draw
    static uint8_t lcdTxRamwrCmd  = ST7789_CMD_RAMWR;
    uint8_t lcdTxRamwrData[] = {
            static_cast<uint8_t>(color >> 8),
            static_cast<uint8_t>(color & 0xFF)
    };
    nrfx_spim_xfer_desc_t lcdXferRamwrCmd  = NRFX_SPIM_XFER_TX(&lcdTxRamwrCmd, 1);
    nrfx_spim_xfer_desc_t lcdXferRamwrData = NRFX_SPIM_XFER_TX(lcdTxRamwrData, sizeof(lcdTxRamwrData));

    setCommandPin();
    APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrCmd, 0));
    setDataPin();
    for (uint32_t i = 0; i < width * height; ++i)
    {
        APP_ERROR_CHECK(nrfx_spim_xfer(&lcdSpi, &lcdXferRamwrData, 0));
    }
}
