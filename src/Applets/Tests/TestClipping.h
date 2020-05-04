#ifndef PYRUS_TESTCLIPPING_H
#define PYRUS_TESTCLIPPING_H

#include "../BaseApplet.h"

#include "../../Widgets/Text.h"

namespace Applet {

class TestClipping: public BaseApplet {
public:
    TestClipping();

    void processEvent(Event::BaseEvent *event) override;
    void update(Platform::BasePlatform &platform) override;
    void draw(Hardware::Screen::BaseScreen &target) override;

    bool allowsStatusBar() const override;
}; // class TestWrap

} // namespace Applet

#endif //PYRUS_TESTCLIPPING_H
