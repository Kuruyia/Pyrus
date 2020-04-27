#ifndef PYRUS_BASEPLATFORM_H
#define PYRUS_BASEPLATFORM_H

#include "../Hardware/BLE/BaseBle.h"
#include "../Hardware/Button/BaseButton.h"
#include "../Hardware/Clock/BaseClock.h"
#include "../Hardware/Screen/BaseScreen.h"

namespace Platform {

class BasePlatform {
public:
    virtual Hardware::BLE::BaseBle &getBleManager() = 0;
    virtual Hardware::Button::BaseButton &getButtonManager() = 0;
    virtual Hardware::Clock::BaseClock &getClockManager() = 0;
    virtual Hardware::Screen::BaseScreen &getScreenManager() = 0;
}; // class BasePlatform

} // namespace Platform

#endif //PYRUS_BASEPLATFORM_H
