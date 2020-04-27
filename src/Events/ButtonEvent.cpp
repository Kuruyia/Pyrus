#include "ButtonEvent.h"

Event::ButtonEvent::ButtonEvent(uint8_t buttonId, bool pressed)
: m_buttonId(buttonId)
, m_pressed(pressed)
{

}

Event::EventIdentifiers Event::ButtonEvent::getEventId() const
{
    return EventIdentifiers::Button;
}

uint8_t Event::ButtonEvent::getButtonId() const
{
    return m_buttonId;
}

bool Event::ButtonEvent::isPressed() const
{
    return m_pressed;
}
