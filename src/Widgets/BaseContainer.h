#ifndef PYRUS_BASECONTAINER_H
#define PYRUS_BASECONTAINER_H

#include <memory>
#include <vector>

#include "BaseWidget.h"

namespace Widget
{

class BaseContainer : public BaseWidget {
public:
    BaseContainer(std::string id, Graphics::Vec2D position, const Graphics::Color &backgroundColor);
    ~BaseContainer() override = default;

    void setPosition(Graphics::Vec2D position) override;

    BaseWidget &addChild(std::unique_ptr<BaseWidget> child);
    bool removeChild(const std::string &id);

    virtual std::unique_ptr<BaseWidget> &findChildById(const std::string &id);
    virtual const std::vector<std::unique_ptr<BaseWidget>> &getChildren();

    virtual void setBackgroundColor(const Graphics::Color &backgroundColor);
    virtual const Graphics::Color &getBackgroundColor() const;

    virtual void setDirtyWithChildren(DirtyState state, bool dirty);

protected:
    std::vector<std::unique_ptr<BaseWidget>> m_children;
    std::function<void(BaseWidget &)> m_newChildHandler;

    Graphics::Color m_backgroundColor;
}; // class BaseContainer

} // namespace Widget

#endif //PYRUS_BASECONTAINER_H
