#ifndef PYRUS_BASEBLE_H
#define PYRUS_BASEBLE_H

#include <Hardware/BLE/Clients/CurrentTime/BaseCurrentTime.h>
#include <Hardware/BLE/Clients/AppleMedia/BaseAppleMedia.h>

namespace Hardware {

namespace BLE {

class BaseBle {
public:
    BaseBle();
    virtual ~BaseBle() = default;

    virtual void init() = 0;

    virtual void deleteBonds() = 0;
    virtual void startAdvertising() = 0;

    virtual bool isConnected() const;

    virtual Clients::BaseCurrentTime &getCurrentTimeClient() = 0;
    virtual Clients::BaseAppleMedia &getAppleMediaClient() = 0;

protected:
    bool m_connected;
}; // class BaseBle

} // namespace BLE

} // namespace Hardware

#endif //PYRUS_BASEBLE_H
