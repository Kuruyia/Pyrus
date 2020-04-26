#ifndef PYRUS_VERTICAL_SCROLL_CONTAINER_H
#define PYRUS_VERTICAL_SCROLL_CONTAINER_H

#include <vector>
#include "BaseContainer.h"

namespace Widget
{

class VerticalScrollContainer : public BaseContainer {
public:
    VerticalScrollContainer(const std::string &id, Vec2D_t position, Vec2D_t size,
            const Graphics::Color &backgroundColor = {0, 0, 0});
    ~VerticalScrollContainer() override = default;

    void draw(Hardware::Screen::BaseScreen &target) override;

    Vec2D_t getAbsolutePosition() const override;

    void setSize(Vec2D_t size);

    uint16_t getWidth() const override;
    uint16_t getHeight() const override;
    Vec2D_t getSize() const override;

    void setVerticalScrollOffset(uint16_t offset);
    uint16_t getVerticalScrollOffset() const;

private:
    Vec2D_t getLastAbsolutePosition() const;
    Graphics::Color getParentBackgroundColor() const;

    Vec2D_t m_size;

    Vec2D_t m_lastPosition;
    Vec2D_t m_lastSize;

    bool m_verticalScrollOffsetChanged;
    uint16_t m_verticalScrollOffset;
}; // class VerticalScrollContainer

} // namespace Widget

#endif // PYRUS_VERTICAL_SCROLL_CONTAINER_H
