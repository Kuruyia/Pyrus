#ifndef PYRUS_VERTICAL_SCROLL_CONTAINER_H
#define PYRUS_VERTICAL_SCROLL_CONTAINER_H

#include <vector>
#include "BaseContainer.h"

namespace Widget
{

class VerticalScrollContainer : public BaseContainer {
public:
    VerticalScrollContainer(const std::string &id, Graphics::Vec2D position, Graphics::Vec2D size,
                            const Graphics::Color &backgroundColor = {0, 0, 0});
    ~VerticalScrollContainer() override = default;

    void draw(Hardware::Screen::BaseScreen &target) override;

    Graphics::Vec2D getAbsolutePosition() const override;

    void setSize(Graphics::Vec2D size);

    int16_t getWidth() const override;
    int16_t getHeight() const override;
    Graphics::Vec2D getSize() const override;

    void setVerticalScrollOffset(uint16_t offset);
    uint16_t getVerticalScrollOffset() const;

private:
    Graphics::Vec2D getLastAbsolutePosition() const;
    Graphics::Color getParentBackgroundColor() const;

    Graphics::Vec2D m_size;

    Graphics::Vec2D m_lastPosition;
    Graphics::Vec2D m_lastSize;

    bool m_verticalScrollOffsetChanged;
    uint16_t m_verticalScrollOffset;
}; // class VerticalScrollContainer

} // namespace Widget

#endif // PYRUS_VERTICAL_SCROLL_CONTAINER_H
