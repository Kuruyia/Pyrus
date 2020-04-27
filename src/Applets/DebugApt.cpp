#include "../Graphics/GfxUtils.h"
#include "../Fonts/Ubuntu24Font.h"

#include "DebugApt.h"

#define APPLET_NAME "DebugApt"

Applet::DebugApt::DebugApt()
: BaseApplet(APPLET_NAME)
, m_clkText("clkText", "--:--", &ubuntu_24ptFontInfo, {16, 48})
, m_bleText("bleText", "BLE Unknown", &ubuntu_24ptFontInfo, {16, 80})
, m_btnText("btnText", "Button Unknown", &ubuntu_24ptFontInfo, {16, 112})
{

}

void Applet::DebugApt::processEvent()
{

}

void Applet::DebugApt::update(Platform::BasePlatform &platform)
{
    // Test the clock
    std::time_t epoch = platform.getClockManager().getTime();
    struct tm *timeinfo = std::localtime(const_cast<const time_t *>(&epoch));

    char timeBuffer[0x9];
    snprintf(timeBuffer, 0x9, "%02u:%02u:%02u", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    m_clkText.setText(timeBuffer);

    // Show BLE state
    m_bleText.setText(platform.getBleManager().isConnected() ? "BLE C" : "BLE D");

    // Update button state
    m_btnText.setText(platform.getButtonManager().isButtonPressed(0) ? "Button ON" : "Button OFF");
}

void Applet::DebugApt::draw(Hardware::Screen::BaseScreen &target)
{
    // Draw widgets
    m_clkText.draw(target);
    m_bleText.draw(target);
    m_btnText.draw(target);
}

bool Applet::DebugApt::allowsStatusBar() const
{
    return true;
}
