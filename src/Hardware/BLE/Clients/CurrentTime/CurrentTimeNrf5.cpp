#include <libraries/util/app_error.h>
#include <libraries/util/sdk_errors.h>
#include <ble/ble_services/ble_cts_c/ble_cts_c.h>

#include "CurrentTimeNrf5.h"
#include "Hardware/BLE/BleNrf5.h"

BLE_CTS_C_DEF(m_ctsClient);

void Hardware::BLE::Clients::CurrentTimeNrf5::initService(BleNrf5 *bluetoothManager)
{
    ble_cts_c_init_t cts_init = {0};

    CallbackCtsEvent<void(ble_cts_c_t *, ble_cts_c_evt_t *)>::func = std::bind(&CurrentTimeNrf5::ctsEventHandler,this,
                                                                                    std::placeholders::_1, std::placeholders::_2);
    auto ctsEventHandlerFunc = static_cast<ble_cts_c_evt_handler_t>(CallbackCtsEvent<void(ble_cts_c_t *, ble_cts_c_evt_t *)>::callback);

    // Initialize the Current Time Service
    cts_init.evt_handler = ctsEventHandlerFunc;
    cts_init.error_handler = ctsErrorHandler;
    cts_init.p_gatt_queue = bluetoothManager->getGattQueueInstance();
    APP_ERROR_CHECK(ble_cts_c_init(&m_ctsClient, &cts_init));

    NRF_SDH_BLE_OBSERVER(m_bleCtsObserver, BLE_CTS_C_BLE_OBSERVER_PRIO, ble_cts_c_on_ble_evt, &m_ctsClient);
}

void Hardware::BLE::Clients::CurrentTimeNrf5::ctsEventHandler(ble_cts_c_t *ctsClient, ble_cts_c_evt_t *ctsEvent)
{
    switch (ctsEvent->evt_type)
    {
        case BLE_CTS_C_EVT_DISCOVERY_COMPLETE:
            APP_ERROR_CHECK(ble_cts_c_handles_assign(&m_ctsClient,
                                                ctsEvent->conn_handle,
                                                &ctsEvent->params.char_handles));

            if (m_eventHandler)
            {
                m_eventHandler({
                                       CurrentTimeEventType::CTS_EVENT_AVAILABLE
                               });
            }
            break;

        case BLE_CTS_C_EVT_DISCOVERY_FAILED:
        case BLE_CTS_C_EVT_DISCONN_COMPLETE:
            if (m_eventHandler)
            {
                m_eventHandler({
                                       CurrentTimeEventType::CTS_EVENT_UNAVAILABLE
                               });
            }
            break;

        case BLE_CTS_C_EVT_CURRENT_TIME:
            // We got the current time, prepare structures to be sent to the callback
            if (m_eventHandler)
            {
                struct tm currentTime {
                    ctsEvent->params.current_time.exact_time_256.day_date_time.date_time.seconds,
                    ctsEvent->params.current_time.exact_time_256.day_date_time.date_time.minutes,
                    ctsEvent->params.current_time.exact_time_256.day_date_time.date_time.hours,
                    ctsEvent->params.current_time.exact_time_256.day_date_time.date_time.day,
                    ctsEvent->params.current_time.exact_time_256.day_date_time.date_time.month,
                    ctsEvent->params.current_time.exact_time_256.day_date_time.date_time.year,
                    ctsEvent->params.current_time.exact_time_256.day_date_time.day_of_week
                };

                CurrentTimeData currentTimeData {
                        currentTime,
                        ctsEvent->params.current_time.exact_time_256.fractions256,
                        {
                            ctsEvent->params.current_time.adjust_reason.manual_time_update > 0,
                            ctsEvent->params.current_time.adjust_reason.external_reference_time_update > 0,
                            ctsEvent->params.current_time.adjust_reason.change_of_time_zone > 0,
                            ctsEvent->params.current_time.adjust_reason.change_of_daylight_savings_time > 0
                        }
                };

                m_eventHandler({
                                       CurrentTimeEventType::CTS_EVENT_TIME_ACQUIRED,
                                       currentTimeData
                               });
            }
            break;

        case BLE_CTS_C_EVT_INVALID_TIME:
            if (m_eventHandler)
            {
                m_eventHandler({
                                       CurrentTimeEventType::CTS_EVENT_TIME_INVALID
                               });
            }
            break;

        default:
            break;
    }
}

void Hardware::BLE::Clients::CurrentTimeNrf5::ctsErrorHandler(uint32_t nrfError)
{
    APP_ERROR_HANDLER(nrfError);
}

ble_cts_c_t *Hardware::BLE::Clients::CurrentTimeNrf5::getCtsClientInstance()
{
    return &m_ctsClient;
}

bool Hardware::BLE::Clients::CurrentTimeNrf5::isAvailable()
{
    return ble_cts_c_is_cts_discovered(&m_ctsClient);
}

void Hardware::BLE::Clients::CurrentTimeNrf5::requestCurrentTime()
{
    ble_cts_c_current_time_read(&m_ctsClient);
}
