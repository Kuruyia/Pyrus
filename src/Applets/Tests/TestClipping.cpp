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

    gfxUtils2.drawFilledTriangle({120, 210}, {100, 230}, {140, 220});

    // Test every clipping axis
    gfxUtils2.setClippingEnabled(true);

    /*
     * Right clipping
     */
    gfxUtils2.setClippingStart({0, 0});
    gfxUtils2.setClippingEnd({18, 320});

    gfxUtils2.drawChar({8, 8}, 'A', ubuntu_24ptFontInfo);
    gfxUtils2.drawLine({8, 88}, {28, 119});
    gfxUtils2.drawLine({28, 88}, {8, 119});
    gfxUtils2.drawFilledCircle({18, 168}, 10);

    gfxUtils2.setClippingStart({140, 0});
    gfxUtils2.setClippingEnd({150, 320});

    gfxUtils2.drawFilledRectangle({140, 8}, {20, 20});
    gfxUtils2.drawFilledTriangle({150, 84}, {140, 104}, {160, 104});

    /*
     * Left clipping
     */
    gfxUtils2.setClippingStart({46, 0});
    gfxUtils2.setClippingEnd({56, 320});

    gfxUtils2.drawChar({36, 8}, 'A', ubuntu_24ptFontInfo);
    gfxUtils2.drawLine({36, 88}, {56, 119});
    gfxUtils2.drawLine({56, 88}, {36, 119});
    gfxUtils2.drawFilledCircle({46, 168}, 10);

    gfxUtils2.setClippingStart({174, 0});
    gfxUtils2.setClippingEnd({184, 320});

    gfxUtils2.drawFilledRectangle({164, 8}, {20, 20});
    gfxUtils2.drawFilledTriangle({174, 84}, {164, 104}, {184, 104});

    /*
     * Bottom clipping
     */
    gfxUtils2.setClippingStart({64, 8});
    gfxUtils2.setClippingEnd({84, 24});
    gfxUtils2.drawChar({64, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({64, 88});
    gfxUtils2.setClippingEnd({84, 104});
    gfxUtils2.drawLine({64, 88}, {84, 119});
    gfxUtils2.drawLine({84, 88}, {64, 119});

    gfxUtils2.setClippingStart({188, 8});
    gfxUtils2.setClippingEnd({208, 18});
    gfxUtils2.drawFilledRectangle({188, 8}, {20, 20});

    gfxUtils2.setClippingStart({188, 84});
    gfxUtils2.setClippingEnd({208, 94});
    gfxUtils2.drawFilledTriangle({198, 84}, {188, 104}, {208, 104});

    gfxUtils2.setClippingStart({64, 158});
    gfxUtils2.setClippingEnd({84, 168});
    gfxUtils2.drawFilledCircle({74, 168}, 10);

    /*
     * Top clipping
     */
    gfxUtils2.setClippingStart({92, 24});
    gfxUtils2.setClippingEnd({112, 40});
    gfxUtils2.drawChar({92, 8}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({92, 104});
    gfxUtils2.setClippingEnd({112, 120});
    gfxUtils2.drawLine({92, 88}, {112, 119});
    gfxUtils2.drawLine({112, 88}, {92, 119});

    gfxUtils2.setClippingStart({212, 18});
    gfxUtils2.setClippingEnd({232, 28});
    gfxUtils2.drawFilledRectangle({212, 8}, {20, 20});

    gfxUtils2.setClippingStart({212, 94});
    gfxUtils2.setClippingEnd({232, 104});
    gfxUtils2.drawFilledTriangle({222, 84}, {212, 104}, {232, 104});

    gfxUtils2.setClippingStart({92, 168});
    gfxUtils2.setClippingEnd({112, 178});
    gfxUtils2.drawFilledCircle({102, 168}, 10);

    /*
     * All-axis clipping
     */
    gfxUtils2.setClippingStart({123, 13});
    gfxUtils2.setClippingEnd({131, 35});
    gfxUtils2.drawChar({120, 8}, '3', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({123, 93});
    gfxUtils2.setClippingEnd({131, 115});
    gfxUtils2.drawLine({120, 88}, {134, 119});
    gfxUtils2.drawLine({134, 88}, {120, 119});

    gfxUtils2.setClippingStart({179, 41});
    gfxUtils2.setClippingEnd({189, 51});
    gfxUtils2.drawFilledRectangle({174, 36}, {20, 20});

    gfxUtils2.setClippingStart({179, 117});
    gfxUtils2.setClippingEnd({189, 127});
    gfxUtils2.drawFilledRectangle({174, 112}, {20, 20});

    gfxUtils2.setClippingStart({41, 201});
    gfxUtils2.setClippingEnd({51, 211});
    gfxUtils2.drawFilledCircle({46, 206}, 10);

    // Test clipping on vertical looping
    gfxUtils2.setLoopVerticalAxis(true);

    /*
     * Right clipping
     */
    gfxUtils2.setClippingStart({0, 320});
    gfxUtils2.setClippingEnd({18, 640});

    gfxUtils2.drawChar({8, 368}, 'A', ubuntu_24ptFontInfo);
    gfxUtils2.drawLine({8, 320+36+88}, {28, 320+36+119});
    gfxUtils2.drawLine({28, 320+36+88}, {8, 320+36+119});

    gfxUtils2.setClippingStart({140, 320});
    gfxUtils2.setClippingEnd({150, 640});

    gfxUtils2.drawFilledRectangle({140, 320+48+8}, {20, 20});
    gfxUtils2.drawFilledTriangle({150, 320+48+84}, {140, 320+48+104}, {160, 320+48+104});
    gfxUtils2.drawFilledCircle({150, 320+168}, 10);

    /*
     * Left clipping
     */
    gfxUtils2.setClippingStart({46, 320});
    gfxUtils2.setClippingEnd({56, 640});

    gfxUtils2.drawChar({36, 368}, 'A', ubuntu_24ptFontInfo);
    gfxUtils2.drawLine({36, 320+36+88}, {56, 320+36+119});
    gfxUtils2.drawLine({56, 320+36+88}, {36, 320+36+119});

    gfxUtils2.setClippingStart({174, 320});
    gfxUtils2.setClippingEnd({184, 640});

    gfxUtils2.drawFilledRectangle({164, 320+48+8}, {20, 20});
    gfxUtils2.drawFilledTriangle({174, 320+48+84}, {164, 320+48+104}, {184, 320+48+104});
    gfxUtils2.drawFilledCircle({174, 320+168}, 10);

    /*
     * Bottom clipping
     */
    gfxUtils2.setClippingStart({64, 368});
    gfxUtils2.setClippingEnd({84, 384});
    gfxUtils2.drawChar({64, 368}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({64, 320+36+88});
    gfxUtils2.setClippingEnd({84, 320+36+104});
    gfxUtils2.drawLine({64, 320+36+88}, {84, 320+36+119});
    gfxUtils2.drawLine({84, 320+36+88}, {64, 320+36+119});

    gfxUtils2.setClippingStart({188, 320+48+8});
    gfxUtils2.setClippingEnd({208, 320+48+18});
    gfxUtils2.drawFilledRectangle({188, 320+48+8}, {20, 20});

    gfxUtils2.setClippingStart({188, 320+48+84});
    gfxUtils2.setClippingEnd({208, 320+48+94});
    gfxUtils2.drawFilledTriangle({198, 320+48+84}, {188, 320+48+104}, {208, 320+48+104});

    gfxUtils2.setClippingStart({188, 320+158});
    gfxUtils2.setClippingEnd({208, 320+168});
    gfxUtils2.drawFilledCircle({198, 320+168}, 10);

    /*
     * Top clipping
     */
    gfxUtils2.setClippingStart({92, 384});
    gfxUtils2.setClippingEnd({112, 400});
    gfxUtils2.drawChar({92, 368}, 'A', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({92, 320+36+104});
    gfxUtils2.setClippingEnd({112, 320+36+120});
    gfxUtils2.drawLine({92, 320+36+88}, {112, 320+36+119});
    gfxUtils2.drawLine({112, 320+36+88}, {92, 320+36+119});

    gfxUtils2.setClippingStart({212, 320+48+18});
    gfxUtils2.setClippingEnd({232, 320+48+28});
    gfxUtils2.drawFilledRectangle({212, 320+48+8}, {20, 20});

    gfxUtils2.setClippingStart({212, 320+48+94});
    gfxUtils2.setClippingEnd({232, 320+48+104});
    gfxUtils2.drawFilledTriangle({222, 320+48+84}, {212, 320+48+104}, {232, 320+48+104});

    gfxUtils2.setClippingStart({212, 320+168});
    gfxUtils2.setClippingEnd({232, 320+178});
    gfxUtils2.drawFilledCircle({222, 320+168}, 10);

    /*
     * All-axis clipping
     */
    gfxUtils2.setClippingStart({123, 373});
    gfxUtils2.setClippingEnd({131, 395});
    gfxUtils2.drawChar({120, 368}, '3', ubuntu_24ptFontInfo);

    gfxUtils2.setClippingStart({123, 320+36+93});
    gfxUtils2.setClippingEnd({131, 320+36+115});
    gfxUtils2.drawLine({120, 320+36+88}, {134, 320+36+119});
    gfxUtils2.drawLine({134, 320+36+88}, {120, 320+36+119});

    gfxUtils2.setClippingStart({189, 320+201});
    gfxUtils2.setClippingEnd({199, 320+211});
    gfxUtils2.drawFilledCircle({194, 320+206}, 10);
}

bool Applet::TestClipping::allowsStatusBar() const
{
    return false;
}
