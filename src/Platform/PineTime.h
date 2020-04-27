#ifndef PYRUS_PINETIME_H
#define PYRUS_PINETIME_H

#include "../Hardware/Button/ButtonPineTime.h"
#include "../Hardware/Clock/ClockNrf52.h"
#include "../Hardware/Screen/ST7789.h"

#include "BasePlatform.h"

namespace Platform {

class PineTime: public BasePlatform {
public:
    PineTime();

    Hardware::BLE::BaseBle &getBleManager() override;
    Hardware::Button::BaseButton &getButtonManager() override;
    Hardware::Clock::BaseClock &getClockManager() override;
    Hardware::Screen::BaseScreen &getScreenManager() override;

private:
    Hardware::Button::ButtonPineTime m_button;
    Hardware::Clock::ClockNrf52 m_clock;
    Hardware::Screen::ST7789 m_lcd;
}; // class PineTime

} // namespace Platform

#endif //PYRUS_PINETIME_H
