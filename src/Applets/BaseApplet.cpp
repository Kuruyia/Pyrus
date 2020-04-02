#include "BaseApplet.h"

Applet::BaseApplet::BaseApplet(const std::string &appletName)
: m_appletName(appletName)
, m_closed(false)
{

}


const std::string &Applet::BaseApplet::getName() const
{
    return m_appletName;
}

bool Applet::BaseApplet::hasAppletSwitch()
{
    return m_switchApplet != nullptr;
}

std::unique_ptr<Applet::BaseApplet> Applet::BaseApplet::acquireAppletSwitch()
{
    return std::move(m_switchApplet);
}

void Applet::BaseApplet::requestAppletSwitch(std::unique_ptr<BaseApplet> applet)
{
    m_switchApplet = std::move(applet);
}

void Applet::BaseApplet::close()
{
    m_closed = true;
}

bool Applet::BaseApplet::isClosed()
{
    return m_closed;
}
