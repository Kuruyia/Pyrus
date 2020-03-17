#ifndef PYRUS_TEXT_H
#define PYRUS_TEXT_H

#include <string>

#include "BaseWidget.h"

namespace Widget
{

class Text : public BaseWidget {
public:
    Text(BaseContainer *parent, const std::string &text, const FONT_INFO *fontInfo, Vec2D_t position,
         Color565_t textColor = {31, 63, 31}, Color565_t backgroundColor = {0, 0, 0});
    ~Text() override;

    void draw(Hardware::Screen::BaseScreen &target) override;

    void setText(const std::string &text);
    const std::string &getText() const;

    void setFont(const FONT_INFO *fontInfo);
    const FONT_INFO *getFont() const;

    void setPosition(Vec2D_t position) override;
    const Vec2D_t &getPosition() const override;

    Vec2D_t getAbsolutePosition() const override;
    const BaseContainer *getParent() const override;

    Vec2D_t getSize() const override;

    void setTextColor(Color565_t textColor);
    const Color565_t &getTextColor() const;

    void setBackgroundColor(Color565_t backgroundColor);
    const Color565_t &getBackgroundColor() const;

    const std::string &getId() const override;

    void markDirty() override;

private:
    Vec2D_t getLastAbsolutePosition() const;
    Color565_t getParentBackgroundColor() const;

    BaseContainer *m_parent;
    const std::string m_id;

    bool m_dirty;
    bool m_clearLastPosition;

    std::string m_text;
    const FONT_INFO *m_fontInfo;

    Vec2D_t m_position;

    Vec2D_t m_lastPosition;
    Vec2D_t m_lastSize;

    Color565_t m_textColor;
    Color565_t m_backgroundColor;
}; // class Text

} // namespace Widget

#endif //PYRUS_TEXT_H
