#include "../../BleNrf5.h"

#include "AppleMediaNrf5.h"

#define AMS_UUID_SERVICE          0x502B
#define AMS_UUID_REMOTE_COMMAND   0x81D8
#define AMS_UUID_ENTITY_UPDATE    0xABCE
#define AMS_UUID_ENTITY_ATTRIBUTE 0xF38C

const ble_uuid128_t bleAmsBaseUuid128 =
{
    {
        // 89D3502B-0F36-433A-8EF4-C502AD55F8DC
        0xDC, 0xF8, 0x55, 0xAD, 0x02, 0xC5, 0xF4, 0x8E,
        0x3A, 0x43, 0x36, 0x0F, 0x2B, 0x50, 0xD3, 0x89
    }
};

const ble_uuid128_t bleAmsRcBaseUuid128 =
{
    {
        // 9B3C81D8-57B1-4A8A-B8DF-0E56F7CA51C2
        0xC2, 0x51, 0xCA, 0xF7, 0x56, 0x0E, 0xDF, 0xB8,
        0x8A, 0x4A, 0xB1, 0x57, 0xD8, 0x81, 0x3C, 0x9B
    }
};

const ble_uuid128_t bleAmsEuBaseUuid128 =
{
    {
        // 2F7CABCE-808D-411F-9A0C-BB92BA96C102
        0x02, 0xC1, 0x96, 0xBA, 0x92, 0xBB, 0x0C, 0x9A,
        0x1F, 0x41, 0x8D, 0x80, 0xCE, 0xAB, 0x7C, 0x2F

    }
};

const ble_uuid128_t bleAmsEaBaseUuid128 =
{
    {
        // C6B2F38C-23AB-46D8-A6AB-A3A870BBD5D7
        0xD7, 0xD5, 0xBB, 0x70, 0xA8, 0xA3, 0xAB, 0xA6,
        0xD8, 0x46, 0xAB, 0x23, 0x8C, 0xF3, 0xB2, 0xC6
    }
};

Hardware::BLE::Clients::AppleMediaNrf5::AppleMediaNrf5()
: m_serviceFound(false)
{

}

uint32_t Hardware::BLE::Clients::AppleMediaNrf5::initService(BleNrf5 *bluetoothManager)
{
    // Make sure that the instance of service is clear. GATT handles inside the service and characteristics are set to @ref BLE_GATT_HANDLE_INVALID.
    memset(&m_amsClientService, 0, sizeof(BleAmsClientService));

    // Assign UUID types.
    VERIFY_SUCCESS(sd_ble_uuid_vs_add(&bleAmsBaseUuid128, &m_amsClientService.service.uuid.type));
    VERIFY_SUCCESS(sd_ble_uuid_vs_add(&bleAmsRcBaseUuid128, &m_amsClientService.remoteCommandChar.uuid.type));
    VERIFY_SUCCESS(sd_ble_uuid_vs_add(&bleAmsEuBaseUuid128, &m_amsClientService.entityUpdateChar.uuid.type));
    VERIFY_SUCCESS(sd_ble_uuid_vs_add(&bleAmsEaBaseUuid128, &m_amsClientService.entityAttributeChar.uuid.type));

    // Assign UUID to the service.
    m_amsClientService.service.uuid.uuid = AMS_UUID_SERVICE;

    return ble_db_discovery_evt_register(&m_amsClientService.service.uuid);
}

void Hardware::BLE::Clients::AppleMediaNrf5::onDbDiscoveryEvent(ble_db_discovery_evt_t *dbDiscoveryEvent)
{
    ble_gatt_db_char_t *dbCharacteristics = dbDiscoveryEvent->params.discovered_db.charateristics;

    // Check whether the AMS Service was discovered.
    if (   (dbDiscoveryEvent->evt_type == BLE_DB_DISCOVERY_COMPLETE)
           && (dbDiscoveryEvent->params.discovered_db.srv_uuid.uuid == AMS_UUID_SERVICE)
           && (dbDiscoveryEvent->params.discovered_db.srv_uuid.type == m_amsClientService.service.uuid.type))
    {
        // Find the handles of the AMS characteristic.
        for (uint32_t i = 0; i < dbDiscoveryEvent->params.discovered_db.char_count; i++)
        {
            switch (dbCharacteristics[i].characteristic.uuid.uuid)
            {
                case AMS_UUID_REMOTE_COMMAND:
                    m_amsClientService.remoteCommandChar = dbCharacteristics[i].characteristic;
                    m_amsClientService.remoteCommandCccd.handle = dbCharacteristics[i].cccd_handle;
                    break;

                case AMS_UUID_ENTITY_UPDATE:
                    m_amsClientService.entityUpdateChar = dbCharacteristics[i].characteristic;
                    m_amsClientService.entityUpdateCccd.handle = dbCharacteristics[i].cccd_handle;
                    break;

                case AMS_UUID_ENTITY_ATTRIBUTE:
                    m_amsClientService.entityAttributeChar = dbCharacteristics[i].characteristic;
                    break;
                default:
                    break;
            }
        }

        // TODO: Send DISCOVERY_COMPLETE event
        m_serviceFound = true;
    }
    else if ((dbDiscoveryEvent->evt_type == BLE_DB_DISCOVERY_SRV_NOT_FOUND) ||
             (dbDiscoveryEvent->evt_type == BLE_DB_DISCOVERY_ERROR))
    {
        // TODO: Send DISCOVERY_FAILED event
        m_serviceFound = false;
    }
    else
    {
        return;
    }
}

bool Hardware::BLE::Clients::AppleMediaNrf5::isAvailable()
{
    return m_serviceFound;
}
