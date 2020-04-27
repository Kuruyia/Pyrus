#include "BaseButton.h"

void Hardware::Button::BaseButton::setButtonHandler(const std::function<void(uint8_t, bool)> &handler)
{
    m_buttonHandler = handler;
}
