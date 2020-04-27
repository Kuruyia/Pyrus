#include <libraries/timer/app_timer.h>

#include "Nrf52Dk.h"

Platform::Nrf52Dk::Nrf52Dk()
: m_lcd({240, 240}, 3, 0xFF, 4, 28, 29, 30)
{
    // Clear the framebuffer
    m_lcd.clearFramebuffer({0, 0, 0});

    // Initialize BLE
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    Hardware::BLE::BaseBle &bleManager = Hardware::BLE::BleNrf5::getInstance();
    bleManager.init();
//    bleManager.deleteBonds();
    bleManager.startAdvertising();
}

Hardware::BLE::BaseBle &Platform::Nrf52Dk::getBleManager()
{
    return Hardware::BLE::BleNrf5::getInstance();
}

Hardware::Clock::BaseClock &Platform::Nrf52Dk::getClockManager()
{
    return m_clock;
}

Hardware::Screen::BaseScreen &Platform::Nrf52Dk::getScreenManager()
{
    return m_lcd;
}
