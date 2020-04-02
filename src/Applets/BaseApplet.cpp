#include "BaseApplet.h"

Applet::BaseApplet::BaseApplet(const std::string &appletName)
: m_appletName(appletName)
{

}


const std::string &Applet::BaseApplet::getName() const
{
    return m_appletName;
}