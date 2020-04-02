#ifndef PYRUS_BASEAPPLET_H
#define PYRUS_BASEAPPLET_H

#include <string>

#include "../Platform/BasePlatform.h"
#include "../Hardware/Screen/BaseScreen.h"

namespace Applet {

class BaseApplet {
public:
    BaseApplet(const std::string &appletName);

    virtual void processEvent() = 0;
    virtual void update(Platform::BasePlatform &platform) = 0;
    virtual void draw(Hardware::Screen::BaseScreen &target) = 0;

    virtual const std::string &getName() const;
    virtual bool showStatusBar() const = 0;

protected:
    const std::string m_appletName;
}; // class BaseApplet

} // namespace BaseApplet

#endif //PYRUS_BASEAPPLET_H
