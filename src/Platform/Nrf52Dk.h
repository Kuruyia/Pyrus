#ifndef PYRUS_NRF52DK_H
#define PYRUS_NRF52DK_H

#include "../Hardware/Button/ButtonNrf52Dk.h"
#include "../Hardware/Clock/ClockNrf52.h"
#include "../Hardware/Screen/ST7789.h"

#include "BasePlatform.h"

namespace Platform {

class Nrf52Dk: public BasePlatform {
public:
    Nrf52Dk();

    Hardware::BLE::BaseBle &getBleManager() override;
    Hardware::Button::BaseButton &getButtonManager() override;
    Hardware::Clock::BaseClock &getClockManager() override;
    Hardware::Screen::BaseScreen &getScreenManager() override;

private:
    Hardware::Button::ButtonNrf52Dk m_button;
    Hardware::Clock::ClockNrf52 m_clock;
    Hardware::Screen::ST7789 m_lcd;
}; // class Nrf52Dk

} // namespace Platform

#endif //PYRUS_NRF52DK_H
