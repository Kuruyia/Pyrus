#include "../Graphics/GfxUtils.h"

#include "TestGfx.h"

#define APPLET_NAME "TestGfx"

Applet::TestGfx::TestGfx()
: BaseApplet(APPLET_NAME)
{

}

void Applet::TestGfx::processEvent()
{

}

void Applet::TestGfx::update(Platform::BasePlatform &platform)
{

}

void Applet::TestGfx::draw(Hardware::Screen::BaseScreen &target)
{
    Graphics::GfxUtils::drawFilledRectangle(target, {16, 48}, {16, 16},
                                            Graphics::Color(255, 0, 0), true);

    Graphics::GfxUtils::drawFilledCircle(target, {32, 84}, 16,
            Graphics::Color(0, 255, 0), Graphics::Color(0, 0, 0),
            true);
}

bool Applet::TestGfx::allowsStatusBar() const
{
    return true;
}
