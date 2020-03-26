#ifndef PYRUS_BLENRF5_H
#define PYRUS_BLENRF5_H

#include <functional>

#include <ble.h>
#include <ble/common/ble_conn_params.h>
#include <ble/peer_manager/peer_manager_types.h>

namespace Hardware
{

namespace BLE
{

class BleNrf5 {
public:
    BleNrf5();

    void deleteBonds();
    void startAdvertising();

private:
    void bleEventHandler(const ble_evt_t *bleEvent);
    static void bleEventHandlerWrapper(const ble_evt_t *bleEvent, void *context);
    void initBleStack();

    void initGapParameters();
    void initGatt();

    static void onAdvertisementEvent(ble_adv_evt_t bleAdvertisementEvent);
    void initAdvertising();

    static void qwrErrorHandler(uint32_t nrfError);
    void initServices();

    void connectionParametersEventHandler(ble_conn_params_evt_t *connectionParametersEvent);
    static void connectionParametersErrorHandler(uint32_t nrfError);
    void initConnectionParameters();

    void peerManagerEventHandler(const pm_evt_t *peerManagerEvent);
    void initPeerManager();

    uint16_t m_connectionHandle;
    static ble_uuid_t m_advertisementUuids[];
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

} // namespace BLE

} // namespace Hardware

#endif //PYRUS_BLENRF5_H
