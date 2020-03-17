#ifndef PYRUS_BASECONTAINER_H
#define PYRUS_BASECONTAINER_H

#include <map>
#include <memory>

#include "BaseWidget.h"

namespace Widget
{

class BaseContainer : public BaseWidget {
public:
    BaseContainer(std::string id, Vec2D_t position, Color565_t backgroundColor);
    ~BaseContainer() override = default;

    virtual void addChild(std::unique_ptr<BaseWidget> child);
    virtual void removeChild(const std::string &id);

    virtual std::unique_ptr<BaseWidget> &findChildById(const std::string &id);

    virtual void setBackgroundColor(Color565_t backgroundColor);
    virtual const Color565_t &getBackgroundColor() const;

protected:
    std::map<std::string, std::unique_ptr<BaseWidget>> m_children;

    Color565_t m_backgroundColor;
}; // class BaseContainer

} // namespace Widget

#endif //PYRUS_BASECONTAINER_H
