#ifndef PYRUS_BASEWIDGET_H
#define PYRUS_BASEWIDGET_H

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
    };

    BaseWidget(std::string id, Vec2D_t position);
    virtual ~BaseWidget() = default;

    virtual void draw(Hardware::Screen::BaseScreen &target) = 0;

    virtual void setPosition(Vec2D_t position);
    virtual const Vec2D_t &getPosition() const;

    virtual Vec2D_t getAbsolutePosition() const = 0;

    virtual void setLoopVerticalPosition(bool loopVerticalPosition);
    virtual bool isLoopingVerticalPosition() const;

    virtual uint16_t getWidth() const = 0;
    virtual uint16_t getHeight() const = 0;
    virtual Vec2D_t getSize() const = 0;

    virtual void setParent(BaseContainer *parent);
    virtual const BaseContainer *getParent() const;

    virtual const std::string &getId() const;

    virtual void setDirty(DirtyState state, bool dirty);
    virtual void clearDirty();
    virtual bool isDirty(DirtyState state);

protected:
    BaseContainer *m_parent;
    const std::string m_id;

    uint16_t m_dirty;

    Vec2D_t m_position;
    bool m_loopVerticalPosition;

}; // class BaseWidget

} // namespace Widget

#endif //PYRUS_BASEWIDGET_H
