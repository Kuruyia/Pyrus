#ifndef PYRUS_TEXT_H
#define PYRUS_TEXT_H

#include <string>

#include "BaseWidget.h"

namespace Widget
{

class Text : public BaseWidget {
public:
    enum HorizontalAlignment {
        Left,
        Centered,
        Right,
    };

    Text(const std::string &id, const std::string &text, const FONT_INFO *fontInfo, Vec2D_t position,
         Color565_t textColor = {31, 63, 31}, Color565_t backgroundColor = {0, 0, 0});
    ~Text() override = default;

    void draw(Hardware::Screen::BaseScreen &target) override;

    void setText(const std::string &text);
    const std::string &getText() const;

    void setFont(const FONT_INFO *fontInfo);
    const FONT_INFO *getFont() const;

    Vec2D_t getAbsolutePosition() const override;
    Vec2D_t getDrawPosition() const;

    uint16_t getWidth() const override;
    uint16_t getHeight() const override;
    Vec2D_t getSize() const override;

    void setTextColor(Color565_t textColor);
    const Color565_t &getTextColor() const;

    void setBackgroundColor(Color565_t backgroundColor);
    const Color565_t &getBackgroundColor() const;

    void setHorizontalAlignment(HorizontalAlignment horizontalAlignment);
    HorizontalAlignment getHorizontalAlignment() const;

private:
    Vec2D_t getLastAbsolutePosition() const;
    Color565_t getParentBackgroundColor() const;

    uint16_t computeWidth() const;

    std::string m_text;
    const FONT_INFO *m_fontInfo;
    HorizontalAlignment m_horizontalAlignment;

    Vec2D_t m_lastDrawPosition;
    Vec2D_t m_lastSize;

    Color565_t m_textColor;
    Color565_t m_backgroundColor;

    uint16_t m_width;

}; // class Text

} // namespace Widget

#endif //PYRUS_TEXT_H
