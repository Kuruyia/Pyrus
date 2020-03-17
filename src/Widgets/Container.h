#ifndef PYRUS_CONTAINER_H
#define PYRUS_CONTAINER_H

#include <vector>
#include "BaseContainer.h"

namespace Widget
{

class Container : public BaseContainer {
public:
    Container(const std::string &id, Vec2D_t position, Vec2D_t size, Color565_t color = {0, 0, 0});
    ~Container() override = default;

    void draw(Hardware::Screen::BaseScreen &target) override;

    void setPosition(Vec2D_t position) override;
    const Vec2D_t &getPosition() const override;

    Vec2D_t getAbsolutePosition() const override;

    void setParent(BaseContainer *parent) override;
    const BaseContainer *getParent() const override;

    void setSize(Vec2D_t size);

    uint16_t getWidth() const override;
    uint16_t getHeight() const override;
    Vec2D_t getSize() const override;

    void setBackgroundColor(Color565_t color) override;
    const Color565_t &getBackgroundColor() const override;

    void addChild(std::unique_ptr<BaseWidget> child) override;
    bool removeChild(const std::string &id) override;

    const std::string &getId() const override;
    std::unique_ptr<BaseWidget> &findChildById(const std::string &id) override;
    const std::vector<std::unique_ptr<BaseWidget>> &getChildren() override;

    void markDirty() override;

private:
    Vec2D_t getLastAbsolutePosition() const;
    Color565_t getParentBackgroundColor() const;

    BaseContainer *m_parent;
    const std::string m_id;

    bool m_dirty;
    bool m_clearLastPosition;

    Vec2D_t m_position;
    Vec2D_t m_size;

    Vec2D_t m_lastPosition;
    Vec2D_t m_lastSize;

    Color565_t m_color;

    std::vector<std::unique_ptr<BaseWidget>> m_children;
}; // class VerticalScrollContainer

} // namespace Widget

#endif //PYRUS_CONTAINER_H
