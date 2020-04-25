#include "AppleMediaNrf5.h"

#include "../../BleNrf5.h"

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
, m_amsClientService()
, m_connectionHandle(BLE_CONN_HANDLE_INVALID)
, m_gattQueue(nullptr)
{
    // Make sure that the instance of service is clear. GATT handles inside the service and characteristics are set to @ref BLE_GATT_HANDLE_INVALID.
    memset(&m_amsClientService, 0, sizeof(BleAmsClientService));
}

uint32_t Hardware::BLE::Clients::AppleMediaNrf5::initService(BleNrf5 *bluetoothManager)
{
    // Store the GATT queue pointer.
    m_gattQueue = bluetoothManager->getGattQueueInstance();

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
    if ((dbDiscoveryEvent->evt_type == BLE_DB_DISCOVERY_COMPLETE)
           && (dbDiscoveryEvent->params.discovered_db.srv_uuid.uuid == AMS_UUID_SERVICE)
           && (dbDiscoveryEvent->params.discovered_db.srv_uuid.type == m_amsClientService.service.uuid.type))
    {
        // Find the handles of the AMS characteristic.
        for (uint32_t i = 0; i < dbDiscoveryEvent->params.discovered_db.char_count; ++i)
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

        // Store the connection handle.
        m_connectionHandle = dbDiscoveryEvent->conn_handle;

        if (m_eventCallback)
            m_eventCallback(AppleMediaEventType::DiscoveryComplete, std::vector<uint8_t>());

        m_serviceFound = true;
    }
    else if ((dbDiscoveryEvent->evt_type == BLE_DB_DISCOVERY_SRV_NOT_FOUND) ||
             (dbDiscoveryEvent->evt_type == BLE_DB_DISCOVERY_ERROR))
    {
        if (m_eventCallback)
            m_eventCallback(AppleMediaEventType::DiscoveryFailed, std::vector<uint8_t>());

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

void Hardware::BLE::Clients::AppleMediaNrf5::onBleEvent(const ble_evt_t *bleEvent)
{
    uint16_t evt = bleEvent->header.evt_id;

    switch (evt)
    {
        case BLE_GATTC_EVT_WRITE_RSP:
            onWriteResponse(bleEvent);
            break;

        case BLE_GATTC_EVT_HVX:
            onGattcNotification(bleEvent);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            onDisconnected(bleEvent);
            break;

        default:
            break;
    }
}

void Hardware::BLE::Clients::AppleMediaNrf5::onWriteResponse(const ble_evt_t *bleEvent)
{
    // Check if the event is on the link for this instance.
    if (m_connectionHandle != bleEvent->evt.gattc_evt.conn_handle)
    {
        return;
    }

    if (bleEvent->evt.gattc_evt.error_handle != BLE_GATT_HANDLE_INVALID
        && bleEvent->evt.gattc_evt.error_handle == m_amsClientService.entityUpdateChar.handle_value)
    {
        onEntityUpdateErrorResponse(bleEvent);
    }
}

void Hardware::BLE::Clients::AppleMediaNrf5::onEntityUpdateErrorResponse(const ble_evt_t *bleEvent)
{
    if (m_eventCallback)
        m_eventCallback(AppleMediaEventType::EntityUpdateWriteError, std::vector<uint8_t>());
}

void Hardware::BLE::Clients::AppleMediaNrf5::onGattcNotification(const ble_evt_t *bleEvent)
{
    const ble_gattc_evt_hvx_t *notification = &bleEvent->evt.gattc_evt.params.hvx;

    if (bleEvent->evt.gattc_evt.conn_handle != m_connectionHandle)
    {
        return;
    }

    if (notification->handle == m_amsClientService.entityUpdateChar.handle_value)
    {
        if (m_eventCallback)
            m_eventCallback(AppleMediaEventType::EntityUpdateNotification, std::vector<uint8_t>(notification->data, notification->data + notification->len));
    }
    else if (notification->handle == m_amsClientService.remoteCommandChar.handle_value)
    {
        if (m_eventCallback)
            m_eventCallback(AppleMediaEventType::RemoteCommandSupportedCmds, std::vector<uint8_t>(notification->data, notification->data + notification->len));
    }
}

void Hardware::BLE::Clients::AppleMediaNrf5::onDisconnected(const ble_evt_t *bleEvent)
{
    if (m_connectionHandle == bleEvent->evt.gap_evt.conn_handle)
    {
        if (m_serviceFound && m_eventCallback)
            m_eventCallback(AppleMediaEventType::Disconnected, std::vector<uint8_t>());

        m_connectionHandle = BLE_CONN_HANDLE_INVALID;
        m_serviceFound = false;
    }
}

uint32_t Hardware::BLE::Clients::AppleMediaNrf5::cccdConfigure(const uint16_t cccdHandle, bool enableNotifications)
{
    nrf_ble_gq_req_t amsClientRequest;
    uint8_t  cccd[BLE_CCCD_VALUE_LEN];
    uint16_t cccdValue = enableNotifications ? BLE_GATT_HVX_NOTIFICATION : 0;

    cccd[0] = LSB_16(cccdValue);
    cccd[1] = MSB_16(cccdValue);

    memset(&amsClientRequest, 0, sizeof(nrf_ble_gq_req_t));

    amsClientRequest.type                        = NRF_BLE_GQ_REQ_GATTC_WRITE;
    amsClientRequest.error_handler.cb            = gattErrorHandler;
    amsClientRequest.error_handler.p_ctx         = nullptr;
    amsClientRequest.params.gattc_write.handle   = cccdHandle;
    amsClientRequest.params.gattc_write.len      = BLE_CCCD_VALUE_LEN;
    amsClientRequest.params.gattc_write.offset   = 0;
    amsClientRequest.params.gattc_write.write_op = BLE_GATT_OP_WRITE_REQ;
    amsClientRequest.params.gattc_write.p_value  = cccd;

    return nrf_ble_gq_item_add(m_gattQueue, &amsClientRequest, m_connectionHandle);
}

void Hardware::BLE::Clients::AppleMediaNrf5::gattErrorHandler(uint32_t nrf_error, void *p_context, uint16_t conn_handle)
{

}

void Hardware::BLE::Clients::AppleMediaNrf5::setEventCallback(
        const std::function<void(AppleMediaEventType, const std::vector<uint8_t> &)> &callback)
{
    m_eventCallback = callback;
}

uint32_t Hardware::BLE::Clients::AppleMediaNrf5::setEntityUpdateNotificationsEnabled(bool enabled)
{
    return cccdConfigure(m_amsClientService.entityUpdateCccd.handle, enabled);
}

uint32_t Hardware::BLE::Clients::AppleMediaNrf5::setEntityUpdateNotificationType(
        Hardware::BLE::Clients::AppleMediaNrf5::AppleMediaEntityID entityId, uint8_t attributeId)
{
    return setEntityUpdateNotificationType(entityId, std::vector<uint8_t>{attributeId});
}

uint32_t Hardware::BLE::Clients::AppleMediaNrf5::setEntityUpdateNotificationType(
        Hardware::BLE::Clients::AppleMediaNrf5::AppleMediaEntityID entityId, const std::vector<uint8_t> &attributeIds)
{
    if (m_connectionHandle == BLE_CONN_HANDLE_INVALID)
        return NRF_ERROR_INVALID_STATE;

    nrf_ble_gq_req_t writeRequest;

    memset(&writeRequest, 0, sizeof(nrf_ble_gq_req_t));

    std::vector<uint8_t> message{static_cast<unsigned char>(entityId)};
    std::copy(attributeIds.begin(), attributeIds.end(), std::back_inserter(message));

    writeRequest.type                        = NRF_BLE_GQ_REQ_GATTC_WRITE;
    writeRequest.error_handler.cb            = gattErrorHandler;
    writeRequest.error_handler.p_ctx         = nullptr;
    writeRequest.params.gattc_write.handle   = m_amsClientService.entityUpdateChar.handle_value;
    writeRequest.params.gattc_write.len      = message.size();
    writeRequest.params.gattc_write.p_value  = &message[0];
    writeRequest.params.gattc_write.offset   = 0;
    writeRequest.params.gattc_write.write_op = BLE_GATT_OP_WRITE_REQ;

    return nrf_ble_gq_item_add(m_gattQueue, &writeRequest, m_connectionHandle);
}

void Hardware::BLE::Clients::AppleMediaNrf5::parseEventDataToEntityUpdate(const std::vector<uint8_t> &data,
                                                                          Hardware::BLE::Clients::AppleMediaNrf5::AppleMediaEntityUpdateEvent &entityUpdateEvent)
{
    if (data.size() < 3) return;

    entityUpdateEvent.entityId = (AppleMediaEntityID) data[0];
    entityUpdateEvent.attributeId = data[1];
    entityUpdateEvent.entityUpdateFlags = data[2];

    if (data.size() > 3)
        entityUpdateEvent.value = std::string(data.begin() + 3, data.end());
}

uint32_t Hardware::BLE::Clients::AppleMediaNrf5::setRemoteCommandNotificationsEnabled(bool enabled)
{
    return cccdConfigure(m_amsClientService.remoteCommandCccd.handle, enabled);
}

uint32_t Hardware::BLE::Clients::AppleMediaNrf5::sendRemoteCommand(
        Hardware::BLE::Clients::AppleMediaNrf5::AppleMediaRemoteCommandID remoteCommandId)
{
    if (m_connectionHandle == BLE_CONN_HANDLE_INVALID)
        return NRF_ERROR_INVALID_STATE;

    nrf_ble_gq_req_t writeRequest;

    memset(&writeRequest, 0, sizeof(nrf_ble_gq_req_t));

    writeRequest.type                        = NRF_BLE_GQ_REQ_GATTC_WRITE;
    writeRequest.error_handler.cb            = gattErrorHandler;
    writeRequest.error_handler.p_ctx         = nullptr;
    writeRequest.params.gattc_write.handle   = m_amsClientService.remoteCommandChar.handle_value;
    writeRequest.params.gattc_write.len      = 1;
    writeRequest.params.gattc_write.p_value  = reinterpret_cast<const uint8_t *>(&remoteCommandId);
    writeRequest.params.gattc_write.offset   = 0;
    writeRequest.params.gattc_write.write_op = BLE_GATT_OP_WRITE_REQ;

    return nrf_ble_gq_item_add(m_gattQueue, &writeRequest, m_connectionHandle);
}
