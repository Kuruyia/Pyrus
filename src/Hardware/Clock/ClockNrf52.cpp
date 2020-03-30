#include "ClockNrf52.h"

#define CLOCK_FREQ 8

Hardware::Clock::ClockNrf52::ClockNrf52()
: m_rtcInstance(NRFX_RTC_INSTANCE(2))
, m_baseEpoch(0)
{
    // Start the low-frequency clock
    APP_ERROR_CHECK(nrfx_clock_init(&clockHandler));
    nrfx_clock_enable();
    nrfx_clock_lfclk_start();

    // Set the RTC configuration to use a custom prescaler value
    nrfx_rtc_config_t rtcConfig = NRFX_RTC_DEFAULT_CONFIG;
    rtcConfig.prescaler = RTC_FREQ_TO_PRESCALER(CLOCK_FREQ);

    // Init the RTC
    CallbackRTC<void(nrfx_rtc_int_type_t)>::func = std::bind(&ClockNrf52::rtcHandler, this, std::placeholders::_1);
    auto rtcHandlerFunc = static_cast<nrfx_rtc_handler_t>(CallbackRTC<void(nrfx_rtc_int_type_t)>::callback);

    APP_ERROR_CHECK(nrfx_rtc_init(&m_rtcInstance, &rtcConfig, rtcHandlerFunc));

    // Enable overflow interrupt
    nrfx_rtc_overflow_enable(&m_rtcInstance, true);

    // Enable the RTC
    nrfx_rtc_enable(&m_rtcInstance);
}

void Hardware::Clock::ClockNrf52::clockHandler(nrfx_clock_evt_type_t eventType)
{

}

void Hardware::Clock::ClockNrf52::rtcHandler(nrfx_rtc_int_type_t intType)
{
    // If the RTC overflows, we must add the overflow to the base time
    if (intType == NRF_DRV_RTC_INT_OVERFLOW)
        m_baseEpoch += (0xFFFFFF / CLOCK_FREQ);
}

void Hardware::Clock::ClockNrf52::setTime(std::time_t time)
{
    m_baseEpoch = time;
    nrfx_rtc_counter_clear(&m_rtcInstance);
}

std::time_t Hardware::Clock::ClockNrf52::getTime() const
{
    return m_baseEpoch + (nrfx_rtc_counter_get(&m_rtcInstance) / CLOCK_FREQ);
}
