#ifndef PYRUS_TEXT_H
#define PYRUS_TEXT_H

#include <string>

#include "Hardware/Screen/BaseScreen.h"

namespace Widget
{

class Text {
public:
    Text(const std::string &text, const FONT_INFO *fontInfo, Vec2D_t position, Color565_t textColor = {31, 63, 31},
         Color565_t backgroundColor = {0, 0, 0});

    void draw(Hardware::Screen::BaseScreen &target);

    void setText(const std::string &text);
    const std::string &getText() const;

    void setFont(const FONT_INFO *fontInfo);
    const FONT_INFO *getFont() const;

    void setPosition(Vec2D_t position);
    const Vec2D_t &getPosition() const;

    void setTextColor(Color565_t textColor);
    const Color565_t &getTextColor() const;

    void setBackgroundColor(Color565_t backgroundColor);
    const Color565_t &getBackgroundColor() const;

private:
    bool m_dirty;
    bool m_clearLastPosition;

    std::string m_text;
    const FONT_INFO *m_fontInfo;

    Vec2D_t m_position;

    Vec2D_t m_lastPosition;
    Vec2D_t m_lastSize;

    Color565_t m_textColor;
    Color565_t m_backgroundColor;
};

}

#endif //PYRUS_TEXT_H
