#include <ctime>
#include <memory>

#include <nrf_delay.h>
#include <nrf_gpio.h>
#include <libraries/button/app_button.h>
#include <nrfx_spim.h>
#include <app_timer.h>
#include <Hardware/BLE/BleNrf5.h>

#include "Platform/PineTime.h"

#include "Fonts/Ubuntu24Font.h"
#include "Widgets/Container.h"
#include "Widgets/Text.h"

void buttonHandler(uint8_t pinNo, uint8_t buttonAction)
{

}

int main()
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
        &buttonHandler
    };

    app_button_init(&pinetimeButton, 1, 10);
    app_button_enable();

    // Reset backlight pins
    nrf_gpio_pin_clear(14);
    nrf_gpio_pin_set(22);
    nrf_gpio_pin_set(23);

    // Start the platform
    Platform::PineTime platform;

    Widget::Text clkText("clkText", "--:--", &ubuntu_24ptFontInfo, {16, 16});
    Widget::Text bleText("bleText", "BLE Unknown", &ubuntu_24ptFontInfo, {16, 48});
    Widget::Text ctsText("ctsText", "CTS Unknown", &ubuntu_24ptFontInfo, {16, 80});

//    bleManager.getCurrentTimeClient().setCurrentTimeEventHandler([&] (const Hardware::BLE::Clients::BaseCurrentTime::CurrentTimeEventData &eventData){
//        switch (eventData.eventType)
//        {
//            case Hardware::BLE::Clients::BaseCurrentTime::CTS_EVENT_AVAILABLE:
//                bleManager.getCurrentTimeClient().requestCurrentTime();
//                ctsText.setText("CTS A");
//                break;
//
//            case Hardware::BLE::Clients::BaseCurrentTime::CTS_EVENT_UNAVAILABLE:
//                ctsText.setText("CTS U");
//                break;
//
//            case Hardware::BLE::Clients::BaseCurrentTime::CTS_EVENT_TIME_ACQUIRED:
//            {
//                struct tm currentTime = eventData.currentTimeData.time;
//                clock.setTime(mktime(&currentTime));
//            }
//                break;
//
//            default:
//                break;
//        }
//    });

    while (true)
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
        std::time_t epoch = platform.getClockManager().getTime();
        struct tm *timeinfo = std::localtime(const_cast<const time_t *>(&epoch));

        char timeBuffer[0x9];
        snprintf(timeBuffer, 0x9, "%02u:%02u:%02u", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        clkText.setText(timeBuffer);
        clkText.draw(platform.getScreenManager());

        // Show BLE state
        bleText.setText(platform.getBleManager().isConnected() ? "BLE C" : "BLE D");
        bleText.draw(platform.getScreenManager());

        ctsText.draw(platform.getScreenManager());

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
