#ifndef PYRUS_CURRENTTIMENRF5_H
#define PYRUS_CURRENTTIMENRF5_H

#include <functional>

#include <ble/ble_services/ble_cts_c/ble_cts_c.h>

#include "BaseCurrentTime.h"

namespace Hardware {

namespace BLE {

class BleNrf5;

namespace Clients {

class CurrentTimeNrf5 : public BaseCurrentTime {
public:
    ~CurrentTimeNrf5() override = default;

    bool isAvailable() override;

    void requestCurrentTime() override;

private:
    friend class BLE::BleNrf5;

    void initService(BleNrf5 *bluetoothManager);

    void ctsEventHandler(ble_cts_c_t *ctsClient, ble_cts_c_evt_t *ctsEvent);
    static void ctsErrorHandler(uint32_t nrfError);

    ble_cts_c_t *getCtsClientInstance();
}; // class CurrentTimeNrf5

// "Convert C++ function pointer to c function pointer": https://stackoverflow.com/a/19809787
template <typename T>
struct CallbackCtsEvent;

template <typename Ret, typename... Params>
struct CallbackCtsEvent<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> CallbackCtsEvent<Ret(Params...)>::func;

} // namespace Clients

} // namespace BLE

} // namespace Hardware

#endif //PYRUS_CURRENTTIMENRF5_H
