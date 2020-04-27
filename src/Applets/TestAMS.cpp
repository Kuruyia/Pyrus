#include <algorithm>
#include <libraries/delay/nrf_delay.h>

#include "../Fonts/Ubuntu24Font.h"
#include "../Hardware/BLE/Clients/AppleMedia/AppleMediaNrf5.h"

#include "TestAMS.h"

#define APPLET_NAME "TestAMS"

Applet::TestAMS::TestAMS(Hardware::BLE::Clients::BaseAppleMedia &appleMedia)
: BaseApplet(APPLET_NAME)
, m_appleMedia(appleMedia)
, m_playPauseSupported(false)
, m_songTitle()
, m_songArtist()
, m_songAlbum()
, m_songDuration()
, m_deviceVolume()
, m_amsText("amsText", "AMS Unknown", &ubuntu_24ptFontInfo, {16, 40})
, m_titleText("evtText", "---", &ubuntu_24ptFontInfo, {16, 90})
, m_subtitleText("subText", "---", &ubuntu_24ptFontInfo, {16, 122}, {200, 200, 200})
, m_metaText("metaText", "---", &ubuntu_24ptFontInfo, {16, 160}, {200, 200, 200})
{
    using namespace Hardware::BLE::Clients;

    appleMedia.setEventCallback([&](AppleMediaNrf5::AppleMediaEventType eventType, const std::vector<uint8_t> &eventData) {
        switch (eventType)
        {
            case AppleMediaNrf5::AppleMediaEventType::DiscoveryComplete:
                m_amsText.setText("AMS Avail");
                appleMedia.setEntityUpdateNotificationsEnabled(true);
                appleMedia.setRemoteCommandNotificationsEnabled(true);

                appleMedia.setEntityUpdateNotificationType(AppleMediaNrf5::AppleMediaEntityID::Track,
                        {AppleMediaNrf5::AppleMediaTrackAttributeID::Title,
                         AppleMediaNrf5::AppleMediaTrackAttributeID::Album,
                         AppleMediaNrf5::AppleMediaTrackAttributeID::Artist,
                         AppleMediaNrf5::AppleMediaTrackAttributeID::Duration});

                appleMedia.setEntityUpdateNotificationType(AppleMediaNrf5::AppleMediaEntityID::Player,
                        {AppleMediaNrf5::AppleMediaPlayerAttributeID ::PlaybackInfo,
                         AppleMediaNrf5::AppleMediaPlayerAttributeID ::Volume});

                break;

            case AppleMediaNrf5::AppleMediaEventType::Disconnected:
                m_amsText.setText("AMS Unavail");
                break;

            case AppleMediaNrf5::AppleMediaEventType::EntityUpdateNotification:
            {
                AppleMediaNrf5::AppleMediaEntityUpdateEvent entityUpdateEvent;
                AppleMediaNrf5::parseEntityUpdateEventData(eventData, entityUpdateEvent);

                if (entityUpdateEvent.entityId == AppleMediaNrf5::AppleMediaEntityID::Track)
                {
                    switch (entityUpdateEvent.attributeId)
                    {
                        case AppleMediaNrf5::AppleMediaTrackAttributeID::Title:
                            m_songTitle = entityUpdateEvent.value;
                            break;

                        case AppleMediaNrf5::AppleMediaTrackAttributeID::Album:
                            m_songAlbum = entityUpdateEvent.value;
                            break;

                        case AppleMediaNrf5::AppleMediaTrackAttributeID::Artist:
                            m_songArtist = entityUpdateEvent.value;
                            break;

                        case AppleMediaNrf5::AppleMediaTrackAttributeID::Duration:
                            m_songDuration = entityUpdateEvent.value;
                            break;
                    }

                    updateInfoTexts();
                }
                else if (entityUpdateEvent.entityId == AppleMediaNrf5::AppleMediaEntityID::Player)
                {
                    switch (entityUpdateEvent.attributeId)
                    {
                        case AppleMediaNrf5::AppleMediaPlayerAttributeID::Volume:
                            m_deviceVolume = stof(entityUpdateEvent.value);
                            break;
                    }

                    updateMetaText();
                }
                break;
            }

            case AppleMediaNrf5::AppleMediaEventType::RemoteCommandSupportedCmds:
                m_playPauseSupported = (std::find(eventData.begin(), eventData.end(), static_cast<uint8_t>(AppleMediaNrf5::AppleMediaRemoteCommandID::TogglePlayPause)) != eventData.end());

                updateMetaText();
                break;

            default:
                break;
        }
    });
}

Applet::TestAMS::~TestAMS()
{
    m_appleMedia.setEventCallback(nullptr);
}

void Applet::TestAMS::processEvent(Event::BaseEvent *event)
{

}

void Applet::TestAMS::update(Platform::BasePlatform &platform)
{

}

void Applet::TestAMS::draw(Hardware::Screen::BaseScreen &target)
{
    m_amsText.draw(target);
    m_titleText.draw(target);
    m_subtitleText.draw(target);
    m_metaText.draw(target);
}

bool Applet::TestAMS::allowsStatusBar() const
{
    return true;
}

void Applet::TestAMS::updateInfoTexts()
{
    m_titleText.setText(m_songTitle);
    m_subtitleText.setText(m_songArtist + " -- " + m_songAlbum);
}

void Applet::TestAMS::updateMetaText()
{
    m_metaText.setText("Vol " + std::to_string((unsigned)(m_deviceVolume * 100)));
    if (m_playPauseSupported)
        m_metaText.setTextColor({0, 255, 0});
    else
        m_metaText.setTextColor({255, 0, 0});
}
