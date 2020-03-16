#ifndef PYRUS_BASECONTAINER_H
#define PYRUS_BASECONTAINER_H

#include "BaseWidget.h"

namespace Widget
{

class BaseContainer : public BaseWidget {
public:
    ~BaseContainer() override = default;

    virtual void addChild(BaseWidget *child) = 0;
    virtual void removeChild(BaseWidget *child) = 0;

    virtual void setBackgroundColor(Color565_t backgroundColor) = 0;
    virtual const Color565_t &getBackgroundColor() const = 0;
};

}

#endif //PYRUS_BASECONTAINER_H
