#ifndef PYRUS_BASECURRENTTIME_H
#define PYRUS_BASECURRENTTIME_H

#include <ctime>
#include <functional>

namespace Hardware {

namespace BLE {

namespace Clients {

class BaseCurrentTime {
public:
    enum CurrentTimeEventType {
        CTS_EVENT_AVAILABLE,
        CTS_EVENT_UNAVAILABLE,
        CTS_EVENT_TIME_ACQUIRED,
        CTS_EVENT_TIME_INVALID
    };

    struct CurrentTimeAdjustReason {
        bool manualTimeUpdate;
        bool externalReferenceTimeUpdate;
        bool timezoneChange;
        bool dstChange;
    };

    struct CurrentTimeData {
        struct tm time;
        uint8_t fractions256;
        CurrentTimeAdjustReason adjustReason;
    };

    struct CurrentTimeEventData {
        CurrentTimeEventType eventType;
        CurrentTimeData currentTimeData;
    };

    virtual ~BaseCurrentTime() = default;

    virtual bool isAvailable() = 0;

    virtual void setCurrentTimeEventHandler(const std::function<void(const CurrentTimeEventData &eventData)> &eventHandler);
    virtual void requestCurrentTime() = 0;

protected:
    std::function<void(const CurrentTimeEventData &eventData)> m_eventHandler;
}; // class BaseCurrentTime

} // namespace Clients

} // namespace BLE

} // namespace Hardware

#endif //PYRUS_BASECURRENTTIME_H
