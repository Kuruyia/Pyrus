#ifndef PYRUS_CLOCKNRF52_H
#define PYRUS_CLOCKNRF52_H

#include <functional>

#include <legacy/nrf_drv_rtc.h>
#include <drivers/include/nrfx_clock.h>

#include "BaseClock.h"

namespace Hardware
{

namespace Clock
{

class ClockNrf52 : public BaseClock {
public:
    ClockNrf52();

    void setTime(std::time_t time) override;
    std::time_t getTime() const override;

private:
    static void clockHandler(nrfx_clock_evt_type_t eventType);
    void rtcHandler(nrfx_rtc_int_type_t intType);

    const nrfx_rtc_t m_rtcInstance;
    std::time_t m_baseEpoch;
}; // class ClockNrf52

// "Convert C++ function pointer to c function pointer": https://stackoverflow.com/a/19809787
template <typename T>
struct CallbackRTC;

template <typename Ret, typename... Params>
struct CallbackRTC<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> CallbackRTC<Ret(Params...)>::func;

} // namespace Clock

} // namespace Hardware

#endif //PYRUS_CLOCKNRF52_H
