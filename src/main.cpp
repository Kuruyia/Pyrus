#include <nrf_delay.h>
#include <nrf_gpio.h>
#include <libraries/button/app_button.h>
#include <nrfx_spim.h>
#include <cstdlib>
#include <ctime>
#include <libraries/log/nrf_log_ctrl.h>
#include <libraries/log/nrf_log_default_backends.h>
#include <libraries/log/nrf_log.h>
#include "Hardware/LCD/ST7789.h"

void buttonHandler(uint8_t pinNo, uint8_t buttonAction)
{

}

int main()
{
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("Pyrus!\n");

    nrf_gpio_cfg_output(20);
    nrf_gpio_pin_clear(20);

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

    // Instanciate a new LCD
    Hardware::LCD::ST7789 lcd(240, 240, 3, 4, 2, 25, 18, 26);
    lcd.drawRectangle(0, 0, 240, 240, 0x5678);

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
