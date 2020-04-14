#include "AppletManager.h"

void Applet::AppletManager::addEvent()
{

}

void Applet::AppletManager::update(Platform::BasePlatform &platform)
{
    if (m_applets.empty()) return;

    if (m_applets.top()->hasAppletSwitch())
    {
        // The current applet wants to switch
        std::unique_ptr<BaseApplet> switchApplet = m_applets.top()->acquireAppletSwitch();
        if (m_applets.top()->isTerminated())
            m_applets.pop();

        m_applets.push(std::move(switchApplet));

        if (m_appletChangedHandler)
            m_appletChangedHandler();
    }
    else
    {
        // Check if the current applet wants to terminate
        if (m_applets.top()->isTerminated())
        {
            m_applets.pop();
            if (m_appletChangedHandler)
                m_appletChangedHandler();
        }
    }

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

void Applet::AppletManager::setAppletChangedHandler(const std::function<void()> &appletChangedHandler)
{
    m_appletChangedHandler = appletChangedHandler;
}
