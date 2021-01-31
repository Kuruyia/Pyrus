#ifndef PYRUS_BASEBUTTON_H
#define PYRUS_BASEBUTTON_H

#include <cinttypes>
#include <functional>

namespace Hardware {

namespace Button {

class BaseButton {
public:
    virtual ~BaseButton() = default;

    virtual void init() = 0;

    virtual bool isButtonPressed(uint8_t buttonId) = 0;

    virtual void setButtonHandler(const std::function<void(uint8_t, bool)> &handler);

protected:
    std::function<void(uint8_t, bool)> m_buttonHandler;
}; // class BaseButton

} // namespace Button

} // namespace Hardware

#endif //PYRUS_BASEBUTTON_H
