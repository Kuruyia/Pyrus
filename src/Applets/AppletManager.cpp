#include "AppletManager.h"

void Applet::AppletManager::addEvent()
{

}

void Applet::AppletManager::update(Platform::BasePlatform &platform)
{
    // TODO: Write event management
    m_applets.top()->processEvent();

    m_applets.top()->update(platform);
}

void Applet::AppletManager::draw(Hardware::Screen::BaseScreen &target)
{
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
