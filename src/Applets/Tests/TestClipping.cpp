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
    if (platform.getScreenManager().getVerticalScrollOffset() != 200)
        platform.getScreenManager().setVerticalScrollOffset(200);
}

void Applet::TestClipping::draw(Hardware::Screen::BaseScreen &target)
{
    Graphics::GfxUtils2 gfxUtils2(target);

    // Test every clipping axis
    gfxUtils2.setClippingEnabled(true);
    gfxUtils2.setClippingStart({0, 0});
    gfxUtils2.setClippingEnd({18, 320});
    gfxUtils2.drawChar({8, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({46, 0});
    gfxUtils2.setClippingEnd({56, 320});
    gfxUtils2.drawChar({36, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({64, 8});
    gfxUtils2.setClippingEnd({84, 24});
    gfxUtils2.drawChar({64, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({92, 24});
    gfxUtils2.setClippingEnd({112, 40});
    gfxUtils2.drawChar({92, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({123, 13});
    gfxUtils2.setClippingEnd({131, 35});
    gfxUtils2.drawChar({120, 8}, '3', ubuntu_24ptFontInfo);

    // Test clipping on vertical looping
    gfxUtils2.setLoopVerticalAxis(true);
    gfxUtils2.setClippingStart({0, 320});
    gfxUtils2.setClippingEnd({18, 640});
    gfxUtils2.drawChar({8, 368}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({46, 320});
    gfxUtils2.setClippingEnd({56, 640});
    gfxUtils2.drawChar({36, 368}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({64, 368});
    gfxUtils2.setClippingEnd({84, 384});
    gfxUtils2.drawChar({64, 368}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({92, 384});
    gfxUtils2.setClippingEnd({112, 400});
    gfxUtils2.drawChar({92, 368}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({123, 373});
    gfxUtils2.setClippingEnd({131, 395});
    gfxUtils2.drawChar({120, 368}, '3', ubuntu_24ptFontInfo);
}

bool Applet::TestClipping::allowsStatusBar() const
{
    return false;
}
