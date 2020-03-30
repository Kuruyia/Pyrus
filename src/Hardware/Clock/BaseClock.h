#ifndef PYRUS_BASECLOCK_H
#define PYRUS_BASECLOCK_H

#include <ctime>

namespace Hardware {

namespace Clock {

class BaseClock {
public:
    virtual ~BaseClock() = default;

    virtual void setTime(std::time_t time) = 0;
    virtual std::time_t getTime() const  = 0;
}; // class BaseClock

} // namespace Clock

} // namespace Hardware

#endif //PYRUS_BASECLOCK_H
