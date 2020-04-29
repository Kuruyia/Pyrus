#include "../../Fonts/Ubuntu24Font.h"

#include "TestWrap.h"

#define APPLET_NAME "TestWrap"

Applet::TestWrap::TestWrap()
: BaseApplet(APPLET_NAME)
, m_wrappedText("wrappedText", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ipsum nunc, pharetra ut lorem a, condimentum imperdiet dui. Nulla facilisis eros nec venenatis vestibulum. Suspendisse nec purus ligula. Donec faucibus, nibh sit amet laoreet molestie, enim magna feugiat enim, et maximus neque leo congue arcu. Sed varius a justo sit amet pretium. Donec hendrerit, dui quis iaculis sagittis, leo nisl euismod purus, sed blandit ante lorem eu nunc. Proin tempus malesuada elit, sed imperdiet tortor malesuada nec. Suspendisse eu tristique lacus, in placerat ipsum. Nunc nunc magna, cursus ac ornare vel, ullamcorper non libero.", &ubuntu_24ptFontInfo, {8, 8})
//, m_wrappedText("wrappedText", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ipsum nunc, pharetra ut lorem a, condimentum imperdiet dui.", &ubuntu_24ptFontInfo, {8, 8})
//, m_wrappedText("wrappedText", "Lorem ipsum", &ubuntu_24ptFontInfo, {8, 340})
{
    m_wrappedText.setWrapMode(Widget::Text::WrapMode::Wrap);
    m_wrappedText.setSizeLimit({240, 320});
    m_wrappedText.setLoopVerticalPosition(true);
}

void Applet::TestWrap::processEvent(Event::BaseEvent *event)
{

}

void Applet::TestWrap::update(Platform::BasePlatform &platform)
{
    if (platform.getScreenManager().getVerticalScrollOffset() != 300)
        platform.getScreenManager().setVerticalScrollOffset(300);
}

void Applet::TestWrap::draw(Hardware::Screen::BaseScreen &target)
{
    m_wrappedText.draw(target);
}

bool Applet::TestWrap::allowsStatusBar() const
{
    return false;
}
