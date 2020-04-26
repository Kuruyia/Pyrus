#ifndef PYRUS_STATUSBAR_H
#define PYRUS_STATUSBAR_H

#include "BaseContainer.h"
#include "Text.h"

namespace Widget {

class StatusBar: public BaseContainer {
public:
    StatusBar(const std::string &id, Graphics::Vec2D position, int16_t width, const FONT_INFO *fontInfo,
              const std::string &primaryText = "", const std::string &secondaryText = "");

    void draw(Hardware::Screen::BaseScreen &target) override;

    void setPosition(Graphics::Vec2D position) override;

    Graphics::Vec2D getAbsolutePosition() const override;

    void setWidth(int16_t width);

    int16_t getWidth() const override;
    int16_t getHeight() const override;
    Graphics::Vec2D getSize() const override;

    Widget::Text &getMainText();
    Widget::Text &getSecondaryText();

private:
    Graphics::Vec2D getLastAbsolutePosition() const;

    Graphics::Vec2D m_size;

    Graphics::Vec2D m_lastPosition;
    Graphics::Vec2D m_lastSize;

    Widget::Text *m_mainText;
    Widget::Text *m_secondaryText;
};

}

#endif //PYRUS_STATUSBAR_H
