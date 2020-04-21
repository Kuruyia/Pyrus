#ifndef PYRUS_APPLEMEDIANRF5_H
#define PYRUS_APPLEMEDIANRF5_H

#include <vector>

#include <ble/ble_db_discovery/ble_db_discovery.h>

namespace Hardware {

namespace BLE {

class BleNrf5;

namespace Clients {

class AppleMediaNrf5 {
public:
    struct BleAmsClientService
    {
        ble_gattc_service_t service;
        ble_gattc_char_t    entityAttributeChar;
        ble_gattc_char_t    remoteCommandChar;
        ble_gattc_desc_t    remoteCommandCccd;
        ble_gattc_char_t    entityUpdateChar;
        ble_gattc_desc_t    entityUpdateCccd;
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

    AppleMediaNrf5();

    bool isAvailable();

    void setEntityUpdateNotificationsEnabled(bool enabled);
    uint32_t setEntityUpdateNotificationType(AppleMediaEntityID entityId, uint8_t attributeId);
    uint32_t setEntityUpdateNotificationType(AppleMediaEntityID entityId, const std::vector<uint8_t> &attributeIds);

private:
    bool m_serviceFound;
    BleAmsClientService m_amsClientService;
    uint16_t m_connectionHandle;
    nrf_ble_gq_t *m_gattQueue;

    friend class BLE::BleNrf5;

    uint32_t initService(BleNrf5 *bluetoothManager);

    void onDbDiscoveryEvent(ble_db_discovery_evt_t *dbDiscoveryEvent);
    void onBleEvent(const ble_evt_t *bleEvent);

    void onWriteResponse(const ble_evt_t *bleEvent);
    void onEntityUpdateErrorResponse(const ble_evt_t *bleEvent);
    void onGattcNotification(const ble_evt_t *bleEvent);
    void onDisconnected(const ble_evt_t *bleEvent);

    static void gattErrorHandler(uint32_t nrf_error, void *p_context, uint16_t conn_handle);

    uint32_t cccdConfigure(uint16_t cccdHandle, bool enableNotifications);
};

}

}

}

#endif //PYRUS_APPLEMEDIANRF5_H
