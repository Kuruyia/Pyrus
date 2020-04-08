#ifndef PYRUS_TESTAPT_H
#define PYRUS_TESTAPT_H

#include "../Widgets/Text.h"

#include "BaseApplet.h"

namespace Applet {

class TestApt: public BaseApplet {
public:
    TestApt();

    void processEvent() override;
    void update(Platform::BasePlatform &platform) override;
    void draw(Hardware::Screen::BaseScreen &target) override;

    bool allowsStatusBar() const override;

private:
    uint32_t m_counter;
    Widget::Text m_counterText;
};

}

#endif //PYRUS_TESTAPT_H
