#ifndef PYRUS_BUTTONEVENT_H
#define PYRUS_BUTTONEVENT_H

#include <cstdint>

#include "BaseEvent.h"

namespace Event {

class ButtonEvent: public BaseEvent {
public:
    ButtonEvent(uint8_t buttonId, bool pressed);
    ~ButtonEvent() override = default;

    EventIdentifiers getEventId() const override;

    uint8_t getButtonId() const;
    bool isPressed() const;

private:
    const uint8_t m_buttonId;
    const bool m_pressed;
}; // class ButtonEvent

} // namespace Event

#endif //PYRUS_BUTTONEVENT_H
