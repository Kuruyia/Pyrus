#ifndef PYRUS_BASECONTAINER_H
#define PYRUS_BASECONTAINER_H

#include <memory>

#include "BaseWidget.h"

namespace Widget
{

class BaseContainer : public BaseWidget {
public:
    ~BaseContainer() override = default;

    virtual void addChild(std::unique_ptr<BaseWidget> child) = 0;
    virtual bool removeChild(const std::string &id) = 0;

    virtual std::unique_ptr<BaseWidget> &findChildById(const std::string &id) = 0;
    virtual const std::vector<std::unique_ptr<BaseWidget>> &getChildren() = 0;

    virtual void setBackgroundColor(Color565_t backgroundColor) = 0;
    virtual const Color565_t &getBackgroundColor() const = 0;
};

}

#endif //PYRUS_BASECONTAINER_H
