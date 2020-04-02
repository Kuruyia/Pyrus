#ifndef PYRUS_APPLICATION_H
#define PYRUS_APPLICATION_H

#include "Applets/AppletManager.h"
#include "Applets/DebugApt.h"

#include "Platform/PineTime.h"
#include "Widgets/Text.h"

class Application {
public:
    Application();

    void run();

private:
    bool m_running;

    Platform::PineTime m_platform;

    Applet::AppletManager m_appletManager;
};

#endif //PYRUS_APPLICATION_H
