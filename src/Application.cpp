#include <cstring>

#include <hal/nrf_gpio.h>
#include <libraries/delay/nrf_delay.h>

#include "Applets/DebugApt.h"
#include "Applets/Tests/TestAMS.h"
#include "Applets/Tests/TestApt.h"
#include "Applets/Tests/TestClipping.h"
#include "Applets/Tests/TestGfx.h"
#include "Applets/Tests/TestWrap.h"

#include "Events/ButtonEvent.h"

#include "Fonts/Ubuntu24Font.h"

#include "Application.h"

Application::Application()
: m_running(true)
, m_statusBar("appletManStatus", {0, 0}, m_platform.getScreenManager().getScreenSize().x,
        &ubuntu_24ptFontInfo, "--:--", "Pyrus")
{
    // Set backlight pins as output
//    nrf_gpio_cfg_output(14);
    nrf_gpio_cfg_output(22);
    nrf_gpio_cfg_output(23);

    // Set battery charge indication pin to input
    nrf_gpio_cfg_input(12, NRF_GPIO_PIN_NOPULL);

    // Set the vibrator pin to output
//    nrf_gpio_cfg_output(16);
    nrf_gpio_pin_set(16);

    // Reset backlight pins
//    nrf_gpio_pin_clear(14);
    nrf_gpio_pin_set(22);
    nrf_gpio_pin_set(23);

    // Setup time synchronisation
    m_platform.getBleManager().getCurrentTimeClient().setCurrentTimeEventHandler(
        [&](const Hardware::BLE::Clients::BaseCurrentTime::CurrentTimeEventData &eventData)
        {
            switch (eventData.eventType)
            {
                case Hardware::BLE::Clients::BaseCurrentTime::CTS_EVENT_AVAILABLE:
                    // Request the current time if CTS became available
                    m_platform.getBleManager().getCurrentTimeClient().requestCurrentTime();
                    break;

                case Hardware::BLE::Clients::BaseCurrentTime::CTS_EVENT_TIME_ACQUIRED:
                {
                    // We received the current time, give it to the clock manager
                    struct tm currentTime = eventData.currentTimeData.time;
                    m_platform.getClockManager().setTime(mktime(&currentTime));
                }
                    break;

                default:
                    break;
            }
        }
    );

    // Pass the button event to the event manager
    m_platform.getButtonManager().setButtonHandler([&](const uint8_t buttonId, const bool pressed) {
        m_eventManager.pushEvent(std::make_unique<Event::ButtonEvent>(buttonId, pressed));
    });

    // Add default applet to the applet manager
    m_appletManager.pushApplet(std::make_unique<Applet::DebugApt>());
    m_statusBar.getSecondaryText().setText(m_appletManager.getCurrentApplet().getName());

    // Set the applet changed handler
    m_appletManager.setAppletChangedHandler([&]() {
        m_statusBar.getSecondaryText().setText(m_appletManager.getCurrentApplet().getName());
        m_platform.getScreenManager().clearFramebuffer({0, 0, 0});
    });
}

void Application::run()
{
    while (m_running)
    {
        // Dim the screen if the battery is charging
        uint32_t isCharging = nrf_gpio_pin_read(12);
        if (!isCharging) {
            nrf_gpio_pin_clear(22);
            nrf_gpio_pin_clear(23);
        } else {
            nrf_gpio_pin_set(22);
            nrf_gpio_pin_set(23);
        }

        // Dispatch events to the current applet
        while (!m_eventManager.isEmpty())
        {
            std::unique_ptr<Event::BaseEvent> event;
            m_eventManager.popEvent(event);

            m_appletManager.dispatchEvent(event.get());
        }

        // Update applets
        m_appletManager.update(m_platform);

        // Update and draw status bar if allowed
        if (m_appletManager.getCurrentApplet().allowsStatusBar())
        {
            // Update the clock
            std::time_t actualEpoch = m_platform.getClockManager().getTime();
            uint8_t actualMinute = (actualEpoch / 60) % 60;
            uint8_t actualHour = (actualEpoch / 3600) % 24;

            char timeBuffer[6];
            snprintf(timeBuffer, 6, "%02u:%02u", actualHour, actualMinute);

            if (strncmp(m_statusBar.getMainText().getText().c_str(), timeBuffer, 6) != 0)
                m_statusBar.getMainText().setText(timeBuffer);

            // Draw the status bar
            m_statusBar.draw(m_platform.getScreenManager());
        }

        // Draw applets
        m_appletManager.draw(m_platform.getScreenManager());

        // Wait 500ms
        nrf_delay_ms(500);
    }

    // Shutdown the screen backlight
    nrf_gpio_pin_set(14);
    nrf_gpio_pin_set(22);
    nrf_gpio_pin_set(23);

    // Completely shutdown the device
    sd_power_system_off();
}
