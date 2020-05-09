#include "../../Fonts/Ubuntu24Font.h"
#include "../../Events/ButtonEvent.h"

#include "TestWrap.h"

#define APPLET_NAME "TestWrap"

#define CLIPPING_SIZE_Y 100

Applet::TestWrap::TestWrap()
: BaseApplet(APPLET_NAME)
, m_wrappedText("wrappedText", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ipsum nunc, pharetra ut lorem a, condimentum imperdiet dui. Nulla facilisis eros nec venenatis vestibulum. Suspendisse nec purus ligula. Donec faucibus, nibh sit amet laoreet molestie, enim magna feugiat enim, et maximus neque leo congue arcu. Sed varius a justo sit amet pretium. Donec hendrerit, dui quis iaculis sagittis, leo nisl euismod purus, sed blandit ante lorem eu nunc. Proin tempus malesuada elit, sed imperdiet tortor malesuada nec. Suspendisse eu tristique lacus, in placerat ipsum. Nunc nunc magna, cursus ac ornare vel, ullamcorper non libero.", &ubuntu_24ptFontInfo, {8, 8})
//, m_wrappedText("wrappedText", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ipsum nunc, pharetra ut lorem a, condimentum imperdiet dui.", &ubuntu_24ptFontInfo, {8, 8})
//, m_wrappedText("wrappedText", "Lorem ipsum", &ubuntu_24ptFontInfo, {8, 340})
, m_offset(0)
, m_offsetIncrement(10)
{
    m_wrappedText.setWrapEnabled(true);
    m_wrappedText.setClippingEnabled(true);
    m_wrappedText.setClippingStart({0, static_cast<int16_t>(m_offset)});
    m_wrappedText.setClippingEnd({240, static_cast<int16_t>(CLIPPING_SIZE_Y + m_offset)});
    m_wrappedText.setLoopVerticalPosition(true);
}

void Applet::TestWrap::processEvent(Event::BaseEvent *event)
{
    using namespace Hardware::BLE::Clients;

    switch (event->getEventId())
    {
        case Event::EventIdentifiers::Button:
        {
            auto *btnEvent = dynamic_cast<Event::ButtonEvent *>(event);
            if (btnEvent->isPressed()) return;

            switch (btnEvent->getButtonId())
            {
                case 0:
                    m_offsetIncrement = -m_offsetIncrement;

                    break;

                case 1:
                    m_offset += m_offsetIncrement;
                    m_wrappedText.setClippingStart({0, static_cast<int16_t>(m_offset)});
                    m_wrappedText.setClippingEnd({240, static_cast<int16_t>(CLIPPING_SIZE_Y + m_offset)});

                    break;
            }

            break;
        }

        default:
            break;
    }
}

void Applet::TestWrap::update(Platform::BasePlatform &platform)
{
    if (platform.getScreenManager().getVerticalScrollOffset() != m_offset)
        platform.getScreenManager().setVerticalScrollOffset(m_offset);
}

void Applet::TestWrap::draw(Hardware::Screen::BaseScreen &target)
{
    m_wrappedText.draw(target);

    printf("done"); // Just to put a breakpoint here
}

bool Applet::TestWrap::allowsStatusBar() const
{
    return false;
}
