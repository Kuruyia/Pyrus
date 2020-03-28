#include "BaseCurrentTime.h"

void Hardware::BLE::Clients::BaseCurrentTime::setCurrentTimeEventHandler(const std::function<void(const CurrentTimeEventData &eventData)> &eventHandler)
{
    m_eventHandler = eventHandler;
}
