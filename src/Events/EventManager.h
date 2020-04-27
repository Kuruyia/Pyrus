#ifndef PYRUS_EVENTMANAGER_H
#define PYRUS_EVENTMANAGER_H

#include <memory>
#include <queue>

#include "BaseEvent.h"

namespace Event {

class EventManager {
public:
    bool isEmpty() const;

    void pushEvent(std::unique_ptr<BaseEvent> event);
    void popEvent(std::unique_ptr<BaseEvent> &event);

private:
    std::queue<std::unique_ptr<BaseEvent>> m_pendingEvents;
}; // class EventManager

} // namespace Event

#endif //PYRUS_EVENTMANAGER_H
