#ifndef PYRUS_STATUSBAR_H
#define PYRUS_STATUSBAR_H

#include "BaseContainer.h"
#include "Text.h"

namespace Widget {

class StatusBar: public BaseContainer {
public:
    StatusBar(const std::string &id, Vec2D_t position, uint16_t width, const FONT_INFO *fontInfo,
            const std::string &primaryText = "", const std::string &secondaryText = "");

    void draw(Hardware::Screen::BaseScreen &target) override;

    void setPosition(Vec2D_t position) override;

    Vec2D_t getAbsolutePosition() const override;

    void setWidth(uint16_t width);

    uint16_t getWidth() const override;
    uint16_t getHeight() const override;
    Vec2D_t getSize() const override;

    Widget::Text &getMainText();
    Widget::Text &getSecondaryText();

private:
    Vec2D_t getLastAbsolutePosition() const;

    Vec2D_t m_size;

    Vec2D_t m_lastPosition;
    Vec2D_t m_lastSize;

    Widget::Text *m_mainText;
    Widget::Text *m_secondaryText;
};

}

#endif //PYRUS_STATUSBAR_H
