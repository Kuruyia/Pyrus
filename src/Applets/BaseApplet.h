#ifndef PYRUS_BASEAPPLET_H
#define PYRUS_BASEAPPLET_H

#include <memory>
#include <string>

#include "../Platform/BasePlatform.h"
#include "../Hardware/Screen/BaseScreen.h"

namespace Applet {

class BaseApplet {
public:
    explicit BaseApplet(const std::string &appletName);

    virtual void processEvent() = 0;
    virtual void update(Platform::BasePlatform &platform) = 0;
    virtual void draw(Hardware::Screen::BaseScreen &target) = 0;

    virtual const std::string &getName() const;
    virtual bool showStatusBar() const = 0;

    bool hasAppletSwitch();
    std::unique_ptr<BaseApplet> acquireAppletSwitch();

    bool isClosed();

protected:
    void requestAppletSwitch(std::unique_ptr<BaseApplet> applet);
    void close();

private:
    const std::string m_appletName;

    std::unique_ptr<BaseApplet> m_switchApplet;
    bool m_closed;
}; // class BaseApplet

} // namespace BaseApplet

#endif //PYRUS_BASEAPPLET_H
