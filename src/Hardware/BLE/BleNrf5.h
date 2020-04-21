#ifndef PYRUS_BLENRF5_H
#define PYRUS_BLENRF5_H

#include <functional>

#include <ble.h>
#include <ble/ble_advertising/ble_advertising.h>
#include <ble/common/ble_conn_params.h>
#include <ble/peer_manager/peer_manager_types.h>

#include "BaseBle.h"
#include "Hardware/BLE/Clients/CurrentTime/CurrentTimeNrf5.h"

namespace Hardware
{

namespace BLE
{

class BleNrf5 : public BaseBle {
public:
    ~BleNrf5() override = default;

    static BleNrf5& getInstance()
    {
        static BleNrf5 instance;
        return instance;
    }

    BleNrf5(BleNrf5 const&) = delete;
    void operator=(BleNrf5 const&) = delete;

    void init() override;

    void deleteBonds() override;
    void startAdvertising() override;

    Clients::BaseCurrentTime &getCurrentTimeClient() override;
    Clients::AppleMediaNrf5 &getAppleMediaClient() override;

private:
    BleNrf5();

    void bleEventHandler(const ble_evt_t *bleEvent);
    static void bleEventHandlerWrapper(const ble_evt_t *bleEvent, void *context);
    void initBleStack();

    void initGapParameters();
    void initGatt();

    static void advertisementEventHandler(ble_adv_evt_t bleAdvertisementEvent);
    void initAdvertising();

    void dbDiscoveryEventHandler(ble_db_discovery_evt_t *dbDiscoveryEvent);
    void initDbDiscovery();
    static void qwrErrorHandler(uint32_t nrfError);
    void initServices();

    void connectionParametersEventHandler(ble_conn_params_evt_t *connectionParametersEvent);
    static void connectionParametersErrorHandler(uint32_t nrfError);
    void initConnectionParameters();

    void peerManagerEventHandler(const pm_evt_t *peerManagerEvent);
    void initPeerManager();

    bool m_initialized;

    uint16_t m_connectionHandle;
    static ble_uuid_t m_advertisementUuids[];

    friend class Clients::CurrentTimeNrf5;
    friend class Clients::AppleMediaNrf5;

    Clients::CurrentTimeNrf5 m_currentTimeClient;
    Clients::AppleMediaNrf5 m_appleMediaClient;

    nrf_ble_gq_t *getGattQueueInstance();
}; // class BleNrf5

/*
 * Yep sorry, that's a mess...
 * "Convert C++ function pointer to c function pointer": https://stackoverflow.com/a/19809787
*/
template <typename T>
struct CallbackConnectionParameters;

template <typename Ret, typename... Params>
struct CallbackConnectionParameters<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> CallbackConnectionParameters<Ret(Params...)>::func;

template <typename T>
struct CallbackPeerManager;

template <typename Ret, typename... Params>
struct CallbackPeerManager<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> CallbackPeerManager<Ret(Params...)>::func;

template <typename T>
struct CallbackDbEvent;

template <typename Ret, typename... Params>
struct CallbackDbEvent<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
    };

// Initialize the static member.
    template <typename Ret, typename... Params>
    std::function<Ret(Params...)> CallbackDbEvent<Ret(Params...)>::func;

} // namespace BLE

} // namespace Hardware

#endif //PYRUS_BLENRF5_H
