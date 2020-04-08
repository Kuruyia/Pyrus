#include "AppletManager.h"

void Applet::AppletManager::addEvent()
{

}

void Applet::AppletManager::update(Platform::BasePlatform &platform)
{
    if (m_applets.empty()) return;

    // Check if the current applet wants to switch
    if (m_applets.top()->hasAppletSwitch())
    {
        std::unique_ptr<BaseApplet> switchApplet = m_applets.top()->acquireAppletSwitch();
        if (m_applets.top()->isClosed())
            m_applets.pop();

        m_applets.push(std::move(switchApplet));
    }

    // Check if the current applet wants to close
    if (m_applets.top()->isClosed())
        m_applets.pop();

    // We potentially popped the last item, so check again
    if (m_applets.empty()) return;

    // TODO: Write event management
    m_applets.top()->processEvent();

    m_applets.top()->update(platform);
}

void Applet::AppletManager::draw(Hardware::Screen::BaseScreen &target)
{
    if (m_applets.empty()) return;

    m_applets.top()->draw(target);
}

void Applet::AppletManager::pushApplet(std::unique_ptr<BaseApplet> applet)
{
    m_applets.push(std::move(applet));
}

void Applet::AppletManager::popApplet()
{
    m_applets.pop();
}

Applet::BaseApplet &Applet::AppletManager::getCurrentApplet()
{
    return *(m_applets.top().get());
}
