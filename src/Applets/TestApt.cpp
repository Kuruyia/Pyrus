#include "../Fonts/Ubuntu24Font.h"

#include "DebugApt.h"
#include "TestApt.h"

#define APPLET_NAME "TestApt"

Applet::TestApt::TestApt()
: BaseApplet(APPLET_NAME)
, m_counter(0)
, m_counterText("ctrText", "0", &ubuntu_24ptFontInfo, {0, 0})
{

}

void Applet::TestApt::processEvent()
{

}

void Applet::TestApt::update(Platform::BasePlatform &platform)
{
    m_counterText.setText(std::to_string(m_counter++));
    if (m_counter > 10)
    {
        requestAppletSwitch(std::make_unique<DebugApt>());
        close();
    }
}

void Applet::TestApt::draw(Hardware::Screen::BaseScreen &target)
{
    m_counterText.draw(target);
}

bool Applet::TestApt::showStatusBar() const
{
    return true;
}
