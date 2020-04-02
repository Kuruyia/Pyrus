#ifndef PYRUS_PINETIME_H
#define PYRUS_PINETIME_H

#include "BasePlatform.h"

namespace Platform {

class PineTime: public BasePlatform {
public:
    PineTime();

    Hardware::BLE::BaseBle &getBleManager() override;
    Hardware::Clock::BaseClock &getClockManager() override;
    Hardware::Screen::BaseScreen &getScreenManager() override;

private:
    Hardware::Clock::ClockNrf52 m_clock;
    Hardware::Screen::ST7789 m_lcd;
}; // class PineTime

} // namespace Platform

#endif //PYRUS_PINETIME_H
