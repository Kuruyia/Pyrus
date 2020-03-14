#ifndef PYRUS_COLOR_H
#define PYRUS_COLOR_H

#include <cstdint>

struct Color565_t
{
    uint8_t r : 5;
    uint8_t g : 6;
    uint8_t b : 5;
};

#endif //PYRUS_COLOR_H
