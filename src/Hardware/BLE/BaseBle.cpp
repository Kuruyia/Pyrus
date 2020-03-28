#include "BaseBle.h"

Hardware::BLE::BaseBle::BaseBle()
: m_connected(false)
{

}

bool Hardware::BLE::BaseBle::isConnected() const
{
    return m_connected;
}
