#ifndef PYRUS_BASECONTAINER_H
#define PYRUS_BASECONTAINER_H

#include <memory>
#include <vector>

#include "BaseWidget.h"

namespace Widget
{

class BaseContainer : public BaseWidget {
public:
    BaseContainer(std::string id, Vec2D_t position, Color565_t backgroundColor);
    ~BaseContainer() override = default;

    void setPosition(Vec2D_t position) override;

    virtual void addChild(std::unique_ptr<BaseWidget> child);
    virtual bool removeChild(const std::string &id);

    virtual std::unique_ptr<BaseWidget> &findChildById(const std::string &id);
    virtual const std::vector<std::unique_ptr<BaseWidget>> &getChildren();

    virtual void setBackgroundColor(Color565_t backgroundColor);
    virtual const Color565_t &getBackgroundColor() const;

    virtual void setDirtyWithChildren(DirtyState state, bool dirty);

protected:
    std::vector<std::unique_ptr<BaseWidget>> m_children;

    Color565_t m_backgroundColor;
}; // class BaseContainer

} // namespace Widget

#endif //PYRUS_BASECONTAINER_H
