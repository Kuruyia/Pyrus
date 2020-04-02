#ifndef PYRUS_APPLICATION_H
#define PYRUS_APPLICATION_H

#include "Platform/PineTime.h"
#include "Widgets/Text.h"

class Application {
public:
    Application();

    void run();

private:
    bool m_running;

    Platform::PineTime m_platform;

    Widget::Text m_clkText;
    Widget::Text m_bleText;
};

#endif //PYRUS_APPLICATION_H
