#include "BaseAppleMedia.h"

void Hardware::BLE::Clients::BaseAppleMedia::setEventCallback(
        const std::function<void(BaseAppleMedia::AppleMediaEventType, const std::vector <uint8_t> &)> &callback)
{
    m_eventCallback = callback;
}

void Hardware::BLE::Clients::BaseAppleMedia::parseEntityUpdateEventData(const std::vector <uint8_t> &data,
                                                BaseAppleMedia::AppleMediaEntityUpdateEvent &entityUpdateEvent)
{
    if (data.size() < 3) return;

    entityUpdateEvent.entityId = (AppleMediaEntityID) data[0];
    entityUpdateEvent.attributeId = data[1];
    entityUpdateEvent.entityUpdateFlags = data[2];

    if (data.size() > 3)
        entityUpdateEvent.value = std::string(data.begin() + 3, data.end());
}
