#ifndef PYRUS_BUTTONPINETIME_H
#define PYRUS_BUTTONPINETIME_H

#include "BaseButton.h"

namespace Hardware {

namespace Button {

class ButtonPineTime: public BaseButton {
public:
    ~ButtonPineTime() override = default;

    void init() override;

    bool isButtonPressed(uint8_t buttonId) override;

private:
    void appButtonHandler(uint8_t pinNumber, uint8_t buttonAction);
}; // class ButtonPineTime

template <typename T>
struct CallbackButton;

template <typename Ret, typename... Params>
struct CallbackButton<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> CallbackButton<Ret(Params...)>::func;

} // namespace Button

} // namespace Hardware

#endif //PYRUS_BUTTONPINETIME_H
