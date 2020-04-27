#ifndef PYRUS_APPLICATION_H
#define PYRUS_APPLICATION_H

#include "Applets/AppletManager.h"
#include "Events/EventManager.h"

#include "Platform/Nrf52Dk.h"

#include "Widgets/StatusBar.h"
#include "Widgets/Text.h"

class Application {
public:
    Application();

    void run();

private:
    bool m_running;

    Platform::Nrf52Dk m_platform;

    Widget::StatusBar m_statusBar;

    Applet::AppletManager m_appletManager;
    Event::EventManager m_eventManager;
};

#endif //PYRUS_APPLICATION_H
