#ifndef PYRUS_BASEPLATFORM_H
#define PYRUS_BASEPLATFORM_H

#include "../Hardware/BLE/BleNrf5.h"
#include "../Hardware/Clock/ClockNrf52.h"
#include "../Hardware/Screen/ST7789.h"

namespace Platform {

class BasePlatform {
public:
    virtual Hardware::BLE::BaseBle &getBleManager() = 0;
    virtual Hardware::Clock::BaseClock &getClockManager() = 0;
    virtual Hardware::Screen::BaseScreen &getScreenManager() = 0;
}; // class BasePlatform

} // namespace Platform

#endif //PYRUS_BASEPLATFORM_H
