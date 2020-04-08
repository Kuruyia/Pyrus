#include <hal/nrf_gpio.h>
#include <libraries/button/app_button.h>
#include <libraries/delay/nrf_delay.h>

#include "Applets/DebugApt.h"
#include "Applets/TestApt.h"

#include "Fonts/Ubuntu24Font.h"

#include "Application.h"

Application::Application()
: m_running(true)
, m_statusBar("appletManStatus", {0, 0}, {m_platform.getScreenManager().getScreenSize().x, 0}, &ubuntu_24ptFontInfo)
{
    // Set backlight pins as output
    nrf_gpio_cfg_output(14);
    nrf_gpio_cfg_output(22);
    nrf_gpio_cfg_output(23);

    // Set battery charge indication pin to input
    nrf_gpio_cfg_input(12, NRF_GPIO_PIN_NOPULL);

    // Set the vibrator pin to output
    nrf_gpio_cfg_output(16);
    nrf_gpio_pin_set(16);

    // Initialize the button
    nrf_gpio_cfg_output(15);
    nrf_gpio_pin_set(15);

    app_button_cfg_t pinetimeButton = {
            13,
            APP_BUTTON_ACTIVE_HIGH,
            NRF_GPIO_PIN_PULLDOWN,
            nullptr
    };

    app_button_init(&pinetimeButton, 1, 10);
    app_button_enable();

    // Reset backlight pins
    nrf_gpio_pin_clear(14);
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

    // Add default applet to the applet manager
    m_appletManager.pushApplet(std::make_unique<Applet::TestApt>());
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

        // Update applets
        m_appletManager.update(m_platform);

        // Draw the status bar if allowed
        if (m_appletManager.getCurrentApplet().allowsStatusBar())
            m_statusBar.draw(m_platform.getScreenManager());

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
