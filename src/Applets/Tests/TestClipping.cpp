#include "../../Fonts/Ubuntu24Font.h"
#include "../../Graphics/GfxUtils2.h"

#include "TestClipping.h"

#define APPLET_NAME "TestWrap"

Applet::TestClipping::TestClipping()
: BaseApplet(APPLET_NAME)
{

}

void Applet::TestClipping::processEvent(Event::BaseEvent *event)
{

}

void Applet::TestClipping::update(Platform::BasePlatform &platform)
{

}

void Applet::TestClipping::draw(Hardware::Screen::BaseScreen &target)
{
    Graphics::GfxUtils2 gfxUtils2(target);

    gfxUtils2.setClippingEnabled(true);
    gfxUtils2.setClippingStart({0, 0});
    gfxUtils2.setClippingEnd({18, 320});

    gfxUtils2.drawChar({8, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({46, 0});
    gfxUtils2.setClippingEnd({56, 320});

    gfxUtils2.drawChar({36, 8}, 'A', ubuntu_24ptFontInfo);
}

bool Applet::TestClipping::allowsStatusBar() const
{
    return false;
}
