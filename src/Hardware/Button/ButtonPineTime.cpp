#include <libraries/button/app_button.h>

#include "ButtonPineTime.h"

void Hardware::Button::ButtonPineTime::init()
{
    nrf_gpio_cfg_output(15);
    nrf_gpio_pin_set(15);

    CallbackButton<void(uint8_t, uint8_t)>::func = std::bind(&ButtonPineTime::appButtonHandler, this,
                                                             std::placeholders::_1, std::placeholders::_2);
    static auto buttonHandlerFunc = static_cast<app_button_handler_t>(CallbackButton<void(uint8_t, uint8_t)>::callback);

    app_button_cfg_t pinetimeButton = {
            13,
            APP_BUTTON_ACTIVE_HIGH,
            NRF_GPIO_PIN_PULLDOWN,
            buttonHandlerFunc
    };

    app_button_init(&pinetimeButton, 1, 10);
    app_button_enable();
}

bool Hardware::Button::ButtonPineTime::isButtonPressed(uint8_t buttonId)
{
    return app_button_is_pushed(buttonId);
}

void Hardware::Button::ButtonPineTime::appButtonHandler(uint8_t pinNumber, uint8_t buttonAction)
{
    if (m_buttonHandler)
        m_buttonHandler(0, buttonAction);
}
