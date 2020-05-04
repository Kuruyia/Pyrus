#ifndef PYRUS_TESTWRAP_H
#define PYRUS_TESTWRAP_H

#include "../BaseApplet.h"

#include "../../Widgets/Text.h"

namespace Applet {

class TestWrap: public BaseApplet {
public:
    TestWrap();

    void processEvent(Event::BaseEvent *event) override;
    void update(Platform::BasePlatform &platform) override;
    void draw(Hardware::Screen::BaseScreen &target) override;

    bool allowsStatusBar() const override;

private:
    Widget::Text m_wrappedText;

    uint16_t m_offset;
    int16_t m_offsetIncrement;
}; // class TestWrap

} // namespace Applet

#endif //PYRUS_TESTWRAP_H
