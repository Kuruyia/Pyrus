#include <hal/nrf_gpio.h>
#include <libraries/button/app_button.h>
#include <libraries/delay/nrf_delay.h>

#include "Fonts/Ubuntu24Font.h"

#include "Application.h"

Application::Application()
: m_running(true)
, m_clkText("clkText", "--:--", &ubuntu_24ptFontInfo, {16, 16})
, m_bleText("bleText", "BLE Unknown", &ubuntu_24ptFontInfo, {16, 48})
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

        // Exit the loop if the button is pressed
        if (app_button_is_pushed(0)) {
            break;
        }

        // Test the clock
        std::time_t epoch = m_platform.getClockManager().getTime();
        struct tm *timeinfo = std::localtime(const_cast<const time_t *>(&epoch));

        char timeBuffer[0x9];
        snprintf(timeBuffer, 0x9, "%02u:%02u:%02u", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        m_clkText.setText(timeBuffer);
        m_clkText.draw(m_platform.getScreenManager());

        // Show BLE state
        m_bleText.setText(m_platform.getBleManager().isConnected() ? "BLE C" : "BLE D");
        m_bleText.draw(m_platform.getScreenManager());

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
