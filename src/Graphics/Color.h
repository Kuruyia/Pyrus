#ifndef PYRUS_COLOR_H
#define PYRUS_COLOR_H

#include <cmath>
#include <cstdint>

namespace Graphics {

class Color {
public:
    Color(uint8_t red, uint8_t green, uint8_t blue)
            : m_red(red)
            , m_green(green)
            , m_blue(blue)
    {}

    template <uint8_t redBits, uint8_t greenBits, uint8_t blueBits>
    Color convertToColorEncoding() const
    {
        return {static_cast<uint8_t>(m_red * std::pow(2, redBits) / 256.f),
                      static_cast<uint8_t>(m_green * std::pow(2, greenBits) / 256.f),
                      static_cast<uint8_t>(m_blue * std::pow(2, blueBits) / 256.f)};
    }

    uint8_t getRed() const
    {
        return m_red;
    }

    uint8_t getGreen() const
    {
        return m_green;
    }

    uint8_t getBlue() const
    {
        return m_blue;
    }

private:
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
}; // class Color

} // namespace Graphics

#endif //PYRUS_COLOR_H
