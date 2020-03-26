#include <ble/peer_manager/peer_manager.h>
#include <ble/ble_advertising/ble_advertising.h>
#include <softdevice/common/nrf_sdh.h>
#include <softdevice/common/nrf_sdh_ble.h>
#include <ble/nrf_ble_qwr/nrf_ble_qwr.h>
#include <ble/ble_services/ble_dis/ble_dis.h>
#include <ble/peer_manager/peer_manager_handler.h>
#include <ble/nrf_ble_gatt/nrf_ble_gatt.h>
#include <app_timer.h>
#include <softdevice/common/nrf_sdh_soc.h>

#include "BleNrf5.h"

#define DEVICE_NAME                     "Pyrus"
#define MANUFACTURER_NAME               "Pine64"
#define DEVICE_MODEL                    "PineTime"
#define FIRMWARE_REVISION               "Pyrus Beta"
#define APP_ADV_INTERVAL                300

#define APP_ADV_DURATION                18000
#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)
#define SLAVE_LATENCY                   0
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)

#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)
#define MAX_CONN_PARAMS_UPDATE_COUNT    3

#define APP_BLE_OBSERVER_PRIO           3
#define APP_BLE_CONN_CFG_TAG            1

NRF_BLE_GATT_DEF(m_gatt);
NRF_BLE_QWR_DEF(m_qwr);
BLE_ADVERTISING_DEF(m_advertising);

ble_uuid_t Hardware::BLE::BleNrf5::m_advertisementUuids[] = {
        {BLE_UUID_DEVICE_INFORMATION_SERVICE, BLE_UUID_TYPE_BLE}
};

Hardware::BLE::BleNrf5::BleNrf5()
: m_connectionHandle(BLE_CONN_HANDLE_INVALID)
{
    initBleStack();
    initGapParameters();
    initGatt();
    initAdvertising();
    initServices();
    initConnectionParameters();
    initPeerManager();
}

void Hardware::BLE::BleNrf5::deleteBonds()
{
    ret_code_t err_code;

    err_code = pm_peers_delete();
    APP_ERROR_CHECK(err_code);
}

void Hardware::BLE::BleNrf5::startAdvertising()
{
    ret_code_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);

    APP_ERROR_CHECK(err_code);
}

void Hardware::BLE::BleNrf5::bleEventHandler(const ble_evt_t *bleEvent)
{
    ret_code_t errorCode = NRF_SUCCESS;

    switch (bleEvent->header.evt_id)
    {
        case BLE_GAP_EVT_DISCONNECTED:
            // LED indication will be changed when advertising starts.
            break;

        case BLE_GAP_EVT_CONNECTED:
            APP_ERROR_CHECK(errorCode);
            m_connectionHandle = bleEvent->evt.gap_evt.conn_handle;
            errorCode = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_connectionHandle);
            APP_ERROR_CHECK(errorCode);
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            const ble_gap_phys_t phys =
            {
                    BLE_GAP_PHY_AUTO,
                    BLE_GAP_PHY_AUTO
            };
            errorCode = sd_ble_gap_phy_update(bleEvent->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(errorCode);
        } break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            errorCode = sd_ble_gap_disconnect(bleEvent->evt.gattc_evt.conn_handle,
                                              BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(errorCode);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            errorCode = sd_ble_gap_disconnect(bleEvent->evt.gatts_evt.conn_handle,
                                              BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(errorCode);
            break;

        default:
            // No implementation needed.
            break;
    }
}

void Hardware::BLE::BleNrf5::bleEventHandlerWrapper(const ble_evt_t *bleEvent, void *context)
{
    // We passed "this" as the context
    static_cast<BleNrf5 *>(context)->bleEventHandler(bleEvent);
}

void Hardware::BLE::BleNrf5::initBleStack()
{
    ret_code_t errorCode;

    errorCode = nrf_sdh_enable_request();
    APP_ERROR_CHECK(errorCode);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ramStart = 0;
    errorCode = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ramStart);
    APP_ERROR_CHECK(errorCode);

    // Enable BLE stack.
    errorCode = nrf_sdh_ble_enable(&ramStart);
    APP_ERROR_CHECK(errorCode);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, bleEventHandlerWrapper, this);
}

void Hardware::BLE::BleNrf5::initGapParameters()
{
    ret_code_t              errorCode;
    ble_gap_conn_params_t   gapConnectionParameters;
    ble_gap_conn_sec_mode_t securityMode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&securityMode);

    errorCode = sd_ble_gap_device_name_set(&securityMode,
                                           (const uint8_t *)DEVICE_NAME,
                                           strlen(DEVICE_NAME));
    APP_ERROR_CHECK(errorCode);

    errorCode = sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_WATCH);
    APP_ERROR_CHECK(errorCode);

    memset(&gapConnectionParameters, 0, sizeof(gapConnectionParameters));

    gapConnectionParameters.min_conn_interval = MIN_CONN_INTERVAL;
    gapConnectionParameters.max_conn_interval = MAX_CONN_INTERVAL;
    gapConnectionParameters.slave_latency     = SLAVE_LATENCY;
    gapConnectionParameters.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    errorCode = sd_ble_gap_ppcp_set(&gapConnectionParameters);
    APP_ERROR_CHECK(errorCode);
}

void Hardware::BLE::BleNrf5::initGatt()
{
    ret_code_t errorCode = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(errorCode);
}

void Hardware::BLE::BleNrf5::onAdvertisementEvent(ble_adv_evt_t bleAdvertisementEvent)
{

}

void Hardware::BLE::BleNrf5::initAdvertising()
{
    ret_code_t             errorCode;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance      = true;
    init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    init.advdata.uuids_complete.uuid_cnt = sizeof(m_advertisementUuids) / sizeof(m_advertisementUuids[0]);
    init.advdata.uuids_complete.p_uuids  = m_advertisementUuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;

    init.evt_handler = onAdvertisementEvent;

    errorCode = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(errorCode);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

void Hardware::BLE::BleNrf5::qwrErrorHandler(uint32_t nrfError)
{
    APP_ERROR_HANDLER(nrfError);
}

void Hardware::BLE::BleNrf5::initServices()
{
    ret_code_t         errorCode;
    nrf_ble_qwr_init_t qwrInit = {0};

    // Initialize Queued Write Module.
    qwrInit.error_handler = qwrErrorHandler;

    errorCode = nrf_ble_qwr_init(&m_qwr, &qwrInit);
    APP_ERROR_CHECK(errorCode);

    ble_dis_init_t disInit;

    // Initialize Device Information Service.
    memset(&disInit, 0, sizeof(disInit));

    disInit.dis_char_rd_sec = SEC_JUST_WORKS;
    ble_srv_ascii_to_utf8(&disInit.manufact_name_str, (char *)MANUFACTURER_NAME);
    ble_srv_ascii_to_utf8(&disInit.model_num_str, (char *)DEVICE_MODEL);
    ble_srv_ascii_to_utf8(&disInit.fw_rev_str, (char *)FIRMWARE_REVISION);

    errorCode = ble_dis_init(&disInit);
    APP_ERROR_CHECK(errorCode);
}

void Hardware::BLE::BleNrf5::connectionParametersEventHandler(ble_conn_params_evt_t *connectionParametersEvent)
{
    ret_code_t errorCode;

    if (connectionParametersEvent->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        errorCode = sd_ble_gap_disconnect(m_connectionHandle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(errorCode);
    }
}

void Hardware::BLE::BleNrf5::connectionParametersErrorHandler(uint32_t nrfError)
{
    APP_ERROR_HANDLER(nrfError);
}

void Hardware::BLE::BleNrf5::initConnectionParameters()
{
    ret_code_t             errorCode;
    ble_conn_params_init_t cpInit;

    memset(&cpInit, 0, sizeof(cpInit));

    CallbackConnectionParameters<void(ble_conn_params_evt_t *)>::func = std::bind(&BleNrf5::connectionParametersEventHandler,
            this, std::placeholders::_1);
    auto connectionParametersHandlerFunc = static_cast<ble_conn_params_evt_handler_t >(CallbackConnectionParameters<void(ble_conn_params_evt_t *)>::callback);

    cpInit.p_conn_params                  = NULL;
    cpInit.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cpInit.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cpInit.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cpInit.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cpInit.disconnect_on_fail             = false;
    cpInit.evt_handler                    = connectionParametersHandlerFunc;
    cpInit.error_handler                  = connectionParametersErrorHandler;

    errorCode = ble_conn_params_init(&cpInit);
    APP_ERROR_CHECK(errorCode);
}

void Hardware::BLE::BleNrf5::peerManagerEventHandler(const pm_evt_t *peerManagerEvent)
{
    pm_handler_on_pm_evt(peerManagerEvent);
    pm_handler_flash_clean(peerManagerEvent);
}

void Hardware::BLE::BleNrf5::initPeerManager()
{
    ble_gap_sec_params_t securityParameters;
    ret_code_t           errorCode;

    errorCode = pm_init();
    APP_ERROR_CHECK(errorCode);

    memset(&securityParameters, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    securityParameters.bond = true;
    securityParameters.mitm = false;
    securityParameters.lesc = 0;
    securityParameters.keypress = 0;
    securityParameters.io_caps = BLE_GAP_IO_CAPS_NONE;
    securityParameters.oob = false;
    securityParameters.min_key_size = 7;
    securityParameters.max_key_size = 16;
    securityParameters.kdist_own.enc = 1;
    securityParameters.kdist_own.id = 1;
    securityParameters.kdist_peer.enc = 1;
    securityParameters.kdist_peer.id = 1;

    errorCode = pm_sec_params_set(&securityParameters);
    APP_ERROR_CHECK(errorCode);

    CallbackPeerManager<void(const pm_evt_t *)>::func = std::bind(&BleNrf5::peerManagerEventHandler, this, std::placeholders::_1);
    auto pmHandlerFunc = static_cast<pm_evt_handler_t>(CallbackPeerManager<void(const pm_evt_t *)>::callback);

    errorCode = pm_register(pmHandlerFunc);
    APP_ERROR_CHECK(errorCode);
}
