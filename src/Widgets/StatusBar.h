#ifndef PYRUS_STATUSBAR_H
#define PYRUS_STATUSBAR_H

#include "BaseWidget.h"
#include "Text.h"

namespace Widget {

class StatusBar: public BaseWidget {
public:
    StatusBar(const std::string &id, Vec2D_t position, Vec2D_t size, const FONT_INFO *fontInfo);

    void draw(Hardware::Screen::BaseScreen &target) override;

    Vec2D_t getAbsolutePosition() const override;

    uint16_t getWidth() const override;
    uint16_t getHeight() const override;
    Vec2D_t getSize() const override;

private:
    Vec2D_t m_size;
    uint16_t m_maxHeight;

    Widget::Text m_mainText;
    Widget::Text m_secondaryText;
};

}

#endif //PYRUS_STATUSBAR_H
