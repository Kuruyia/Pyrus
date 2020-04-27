#include <libraries/delay/nrf_delay.h>
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
    Graphics::GfxUtils::drawLine(target, {68, 68}, {84, 100}, {255, 255, 0});
    Graphics::GfxUtils::drawLine(target, {106, 68}, {90, 100}, {255, 255, 0});
    Graphics::GfxUtils::drawLine(target, {112, 100}, {128, 68}, {255, 255, 0});
    Graphics::GfxUtils::drawLine(target, {150, 100}, {134, 68}, {255, 255, 0});

    Graphics::GfxUtils::drawLine(target, {0, 40}, {239, 239}, {255, 0, 255});

    Graphics::GfxUtils::drawHorizontalLine(target, {67, 101}, 68, {255, 0, 0});
    Graphics::GfxUtils::drawVerticalLine(target, {67, 67}, 34, {255, 0, 0});

    Graphics::GfxUtils::drawFilledRectangle(target, {16, 48}, {16, 16},
                                            Graphics::Color(255, 0, 0));

    Graphics::GfxUtils::drawFilledCircle(target, {32, 84}, 16,
            Graphics::Color(0, 255, 0), Graphics::Color(0, 0, 0));

    Graphics::GfxUtils::drawTriangle(target, {80, 150}, {100, 200}, {140, 170}, {0, 255, 0});
    Graphics::GfxUtils::drawFilledTriangle(target, {40, 120}, {60, 190}, {100, 150}, {0, 255, 255});
    Graphics::GfxUtils::drawFastFilledTriangle(target, {120, 120}, {140, 190}, {180, 150}, {0, 255, 255});
}

bool Applet::TestGfx::allowsStatusBar() const
{
    return true;
}
