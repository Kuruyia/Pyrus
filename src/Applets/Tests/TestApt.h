#ifndef PYRUS_TESTAPT_H
#define PYRUS_TESTAPT_H

#include <Widgets/StatusBar.h>
#include "Widgets/Text.h"

#include "Applets/BaseApplet.h"

namespace Applet {

class TestApt: public BaseApplet {
public:
    TestApt();

    void processEvent(Event::BaseEvent *event) override;
    void update(Platform::BasePlatform &platform) override;
    void draw(Hardware::Screen::BaseScreen &target) override;

    bool allowsStatusBar() const override;

private:
    uint16_t m_counter;
    Widget::Text m_counterText;
    Widget::StatusBar m_statusBarTest;
};

}

#endif //PYRUS_TESTAPT_H
