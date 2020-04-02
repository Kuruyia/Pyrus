#include <libraries/timer/app_timer.h>

#include "PineTime.h"

Platform::PineTime::PineTime()
: m_lcd({240, 240}, 3, 4, 2, 25, 18, 26)
{
    // Clear the framebuffer
    m_lcd.clearFramebuffer({0, 0, 0});

    // Initialize BLE
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    Hardware::BLE::BaseBle &bleManager = getBleManager();
    bleManager.init();
    bleManager.deleteBonds();
    bleManager.startAdvertising();
}

Hardware::BLE::BaseBle &Platform::PineTime::getBleManager()
{
    return Hardware::BLE::BleNrf5::getInstance();
}

Hardware::Clock::BaseClock &Platform::PineTime::getClockManager()
{
    return m_clock;
}

Hardware::Screen::BaseScreen &Platform::PineTime::getScreenManager()
{
    return m_lcd;
}
