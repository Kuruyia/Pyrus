#ifndef PYRUS_APPLETMANAGER_H
#define PYRUS_APPLETMANAGER_H

#include <memory>
#include <stack>

#include "BaseApplet.h"

namespace Applet {

class AppletManager {
public:
    void addEvent();

    void update(Platform::BasePlatform &platform);
    void draw(Hardware::Screen::BaseScreen &target);

    void pushApplet(std::unique_ptr<BaseApplet> applet);
    void popApplet();

private:
    std::stack<std::unique_ptr<BaseApplet>> m_applets;
}; // class AppletManager

} // namespace Applet

#endif //PYRUS_APPLETMANAGER_H
