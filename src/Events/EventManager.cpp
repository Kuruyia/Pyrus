#include "EventManager.h"

bool Event::EventManager::isEmpty() const
{
    return m_pendingEvents.empty();
}


void Event::EventManager::pushEvent(std::unique_ptr<BaseEvent> event)
{
    m_pendingEvents.push(std::move(event));
}

void Event::EventManager::popEvent(std::unique_ptr<BaseEvent> &event)
{
    event = std::move(m_pendingEvents.front());
    m_pendingEvents.pop();
}