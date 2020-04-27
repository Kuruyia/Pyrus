#include <libraries/button/app_button.h>

#include "ButtonNrf52Dk.h"

#define FIRST_BUTTON_NUMBER 13

void Hardware::Button::ButtonNrf52Dk::init()
{
    CallbackButton<void(uint8_t, uint8_t)>::func = std::bind(&ButtonNrf52Dk::appButtonHandler, this,
                                                             std::placeholders::_1, std::placeholders::_2);
    static auto buttonHandlerFunc = static_cast<app_button_handler_t>(CallbackButton<void(uint8_t, uint8_t)>::callback);

    static const app_button_cfg_t buttons[] = {
            {
                    13,
                    APP_BUTTON_ACTIVE_LOW,
                    NRF_GPIO_PIN_PULLUP,
                    buttonHandlerFunc
            },
            {
                    14,
                    APP_BUTTON_ACTIVE_LOW,
                    NRF_GPIO_PIN_PULLUP,
                    buttonHandlerFunc
            },
            {
                    15,
                    APP_BUTTON_ACTIVE_LOW,
                    NRF_GPIO_PIN_PULLUP,
                    buttonHandlerFunc
            },
            {
                    16,
                    APP_BUTTON_ACTIVE_LOW,
                    NRF_GPIO_PIN_PULLUP,
                    buttonHandlerFunc
            },
    };

    app_button_init(buttons, 4, 10);
    app_button_enable();
}

bool Hardware::Button::ButtonNrf52Dk::isButtonPressed(uint8_t buttonId)
{
    return app_button_is_pushed(buttonId);
}

void Hardware::Button::ButtonNrf52Dk::appButtonHandler(uint8_t pinNumber, uint8_t buttonAction)
{
    if (m_buttonHandler)
        m_buttonHandler(pinNumber - FIRST_BUTTON_NUMBER, buttonAction);
}
