#ifndef PYRUS_TESTGFX_H
#define PYRUS_TESTGFX_H

#include "BaseApplet.h"

namespace Applet {

class TestGfx: public BaseApplet {
public:
    TestGfx();

    void processEvent(Event::BaseEvent *event) override;
    void update(Platform::BasePlatform &platform) override;
    void draw(Hardware::Screen::BaseScreen &target) override;

    bool allowsStatusBar() const override;
}; // class TestGfx

} // namespace Applet

#endif //PYRUS_TESTGFX_H
