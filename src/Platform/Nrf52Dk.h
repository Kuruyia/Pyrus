#ifndef PYRUS_NRF52DK_H
#define PYRUS_NRF52DK_H

#include "BasePlatform.h"

namespace Platform {

class Nrf52Dk: public BasePlatform {
public:
    Nrf52Dk();

    Hardware::BLE::BaseBle &getBleManager() override;
    Hardware::Clock::BaseClock &getClockManager() override;
    Hardware::Screen::BaseScreen &getScreenManager() override;

private:
    Hardware::Clock::ClockNrf52 m_clock;
    Hardware::Screen::ST7789 m_lcd;
}; // class Nrf52Dk

} // namespace Platform

#endif //PYRUS_NRF52DK_H
