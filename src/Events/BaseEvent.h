#ifndef PYRUS_BASEEVENT_H
#define PYRUS_BASEEVENT_H

#include "EventIdentifiers.h"

namespace Event {

class BaseEvent {
public:
    virtual ~BaseEvent() = default;

    virtual EventIdentifiers getEventId() const = 0;
}; // class BaseEvent

} // namespace Event

#endif //PYRUS_BASEEVENT_H
