#ifndef PYRUS_APPLEMEDIANRF5_H
#define PYRUS_APPLEMEDIANRF5_H

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

    AppleMediaNrf5();

    bool isAvailable();

private:
    bool m_serviceFound;
    BleAmsClientService m_amsClientService;
    uint16_t m_connectionHandle;

    friend class BLE::BleNrf5;

    uint32_t initService(BleNrf5 *bluetoothManager);

    void onDbDiscoveryEvent(ble_db_discovery_evt_t *dbDiscoveryEvent);
    void onBleEvent(const ble_evt_t *bleEvent);

    void onWriteResponse(const ble_evt_t *bleEvent);
    void onEntityUpdateErrorResponse(const ble_evt_t *bleEvent);
    void onGattcNotification(const ble_evt_t *bleEvent);
    void onDisconnected(const ble_evt_t *bleEvent);
};

}

}

}

#endif //PYRUS_APPLEMEDIANRF5_H
