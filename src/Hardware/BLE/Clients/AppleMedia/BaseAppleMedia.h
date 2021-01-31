#ifndef PYRUS_BASEAPPLEMEDIA_H
#define PYRUS_BASEAPPLEMEDIA_H

#include <functional>
#include <string>
#include <vector>

namespace Hardware {

namespace BLE {

namespace Clients {

class BaseAppleMedia {
public:
    enum AppleMediaRemoteCommandID
    {
        Play,
        Pause,
        TogglePlayPause,
        NextTrack,
        PreviousTrack,
        VolumeUp,
        VolumeDown,
        AdvanceRepeatMode,
        AdvanceShuffleMode,
        SkipForward,
        SkipBackward,
        LikeTrack,
        DislikeTrack,
        BookmarkTrack
    };

    enum AppleMediaEntityID
    {
        Player = 0,
        Queue,
        Track
    };

    enum AppleMediaPlayerAttributeID
    {
        Name = 0,
        PlaybackInfo,
        Volume
    };

    enum AppleMediaQueueAttributeID
    {
        Index = 0,
        Count,
        ShuffleMode,
        RepeatMode
    };

    enum AppleMediaTrackAttributeID
    {
        Artist = 0,
        Album,
        Title,
        Duration
    };

    enum AppleMediaEntityUpdateFlags
    {
        FlagTruncated = (1 << 0)
    };

    enum AppleMediaEventType
    {
        DiscoveryComplete,
        DiscoveryFailed,
        Disconnected,
        EntityUpdateWriteError,
        EntityUpdateNotification,
        RemoteCommandSupportedCmds
    };

    struct AppleMediaEntityUpdateEvent
    {
        AppleMediaEntityID entityId;
        uint8_t attributeId;
        uint8_t entityUpdateFlags;
        std::string value;
    };

public:
    virtual bool isAvailable() = 0;

    virtual void setEventCallback(const std::function<void(AppleMediaEventType, const std::vector<uint8_t> &)> &callback);

    virtual uint32_t setEntityUpdateNotificationsEnabled(bool enabled) = 0;
    virtual uint32_t setEntityUpdateNotificationType(AppleMediaEntityID entityId, uint8_t attributeId) = 0;
    virtual uint32_t setEntityUpdateNotificationType(AppleMediaEntityID entityId, const std::vector<uint8_t> &attributeIds) = 0;

    static void parseEntityUpdateEventData(const std::vector<uint8_t> &data, AppleMediaEntityUpdateEvent &entityUpdateEvent);

    virtual uint32_t setRemoteCommandNotificationsEnabled(bool enabled) = 0;
    virtual uint32_t sendRemoteCommand(AppleMediaRemoteCommandID remoteCommandId) = 0;

protected:
    std::function<void(AppleMediaEventType, const std::vector<uint8_t> &)> m_eventCallback;
}; // class BaseAppleMedia

} // namespace Clients

} // namespace BLE

} // namespace Hardware

#endif //PYRUS_BASEAPPLEMEDIA_H
