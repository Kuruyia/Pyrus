#ifndef PYRUS_BASEWIDGET_H
#define PYRUS_BASEWIDGET_H

#include <bitset>
#include <functional>
#include <string>
#include <vector>

#include "Hardware/Screen/BaseScreen.h"

namespace Widget
{

class BaseContainer;

class BaseWidget {
public:
    enum DirtyState {
        Global = 0,
        Position,
        Size,
        Color,
        Child,
    };

    BaseWidget(std::string id, Graphics::Vec2D position);
    virtual ~BaseWidget() = default;

    virtual void draw(Hardware::Screen::BaseScreen &target) = 0;

    virtual void setPosition(Graphics::Vec2D position);
    virtual const Graphics::Vec2D &getPosition() const;

    virtual Graphics::Vec2D getAbsolutePosition() const = 0;

    virtual void setLoopVerticalPosition(bool loopVerticalPosition);
    virtual bool isLoopingVerticalPosition() const;

    virtual int16_t getWidth() const = 0;
    virtual int16_t getHeight() const = 0;
    virtual Graphics::Vec2D getSize() const = 0;

    virtual void setParent(BaseContainer *parent);
    virtual const BaseContainer *getParent() const;

    virtual const std::string &getId() const;

    virtual void setDirty(DirtyState state, bool dirty);
    virtual void clearDirty();
    virtual bool isDirty(DirtyState state);

protected:
    virtual Graphics::Color getParentBackgroundColor() const;

    BaseContainer *m_parent;
    const std::string m_id;

    std::bitset<16> m_dirty;

    Graphics::Vec2D m_position;
    bool m_loopVerticalPosition;

}; // class BaseWidget

} // namespace Widget

#endif //PYRUS_BASEWIDGET_H
