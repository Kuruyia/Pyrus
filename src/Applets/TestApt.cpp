#include "../Fonts/Ubuntu24Font.h"

#include "DebugApt.h"
#include "TestApt.h"

#define APPLET_NAME "TestApt"

Applet::TestApt::TestApt()
: BaseApplet(APPLET_NAME)
, m_counter(0)
, m_counterText("ctrText", "0", &ubuntu_24ptFontInfo, {120, 64})
, m_statusBarTest("testStatus", {0, 128}, 240,
                  &ubuntu_24ptFontInfo, "P", "S")
{
    m_counterText.setHorizontalAlignment(Widget::Text::HorizontalAlignment::Centered);
}

void Applet::TestApt::processEvent(Event::BaseEvent *event)
{

}

void Applet::TestApt::update(Platform::BasePlatform &platform)
{
    const unsigned delay = 15;
    m_counterText.setText(std::to_string((1 << delay) >> m_counter++));
    if (m_counter > delay)
    {
        requestAppletSwitch(std::make_unique<DebugApt>());
        terminate();
    }
}

void Applet::TestApt::draw(Hardware::Screen::BaseScreen &target)
{
    m_counterText.draw(target);
    m_statusBarTest.draw(target);
}

bool Applet::TestApt::allowsStatusBar() const
{
    return true;
}
