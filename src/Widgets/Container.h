#ifndef PYRUS_CONTAINER_H
#define PYRUS_CONTAINER_H

#include <vector>
#include "BaseContainer.h"

namespace Widget
{

class Container : public BaseContainer {
public:
    Container(const std::string &id, Vec2D_t position, Vec2D_t size,
            const Graphics::Color &backgroundColor = {0, 0, 0});
    ~Container() override = default;

    void draw(Hardware::Screen::BaseScreen &target) override;

    Vec2D_t getAbsolutePosition() const override;

    void setSize(Vec2D_t size);

    uint16_t getWidth() const override;
    uint16_t getHeight() const override;
    Vec2D_t getSize() const override;

private:
    Vec2D_t getLastAbsolutePosition() const;
    Graphics::Color getParentBackgroundColor() const;

    Vec2D_t m_size;

    Vec2D_t m_lastPosition;
    Vec2D_t m_lastSize;
}; // class VerticalScrollContainer

} // namespace Widget

#endif //PYRUS_CONTAINER_H
