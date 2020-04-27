#ifndef PYRUS_BACKEVENT_H
#define PYRUS_BACKEVENT_H

#include "BaseEvent.h"

namespace Event {

class BackEvent: public BaseEvent {
public:
    ~BackEvent() override = default;

    EventIdentifiers getEventId() const override;
}; // class BackEvent

} // namespace Event

#endif //PYRUS_BACKEVENT_H
