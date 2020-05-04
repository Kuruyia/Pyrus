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

    enum WrapMode {
        None,
        Wrap
    };

    Text(const std::string &id, const std::string &text, const FONT_INFO *fontInfo, Graphics::Vec2D position,
         const Graphics::Color &textColor = {255, 255, 255},
         const Graphics::Color &backgroundColor = {0, 0, 0});
    ~Text() override = default;

    void draw(Hardware::Screen::BaseScreen &target) override;

    void setText(const std::string &text);
    const std::string &getText() const;

    void setFont(const FONT_INFO *fontInfo);
    const FONT_INFO *getFont() const;

    Graphics::Vec2D getAbsolutePosition() const override;
    Graphics::Vec2D getDrawPosition() const;

    int16_t getWidth() const override;
    int16_t getHeight() const override;
    Graphics::Vec2D getSize() const override;

    void setTextColor(const Graphics::Color &textColor);
    const Graphics::Color &getTextColor() const;

    void setBackgroundColor(const Graphics::Color &backgroundColor);
    const Graphics::Color &getBackgroundColor() const;

    void setHorizontalAlignment(HorizontalAlignment horizontalAlignment);
    HorizontalAlignment getHorizontalAlignment() const;

    void setWrapMode(WrapMode wrapMode);
    WrapMode getWrapMode() const;

    void setSizeLimit(const Graphics::Vec2D &sizeLimit);
    const Graphics::Vec2D &getSizeLimit() const;

    void setStartHeight(uint16_t startHeight);
    uint16_t getStartHeight() const;

private:
    enum DirtyStateExtension
    {
        StartHeight = DirtyState::End,
    };

    Graphics::Vec2D getLastAbsolutePosition() const;

    uint16_t computeWidth(const std::string &str) const;
    uint16_t computeWidth() const;

    void drawAndGetSize(Hardware::Screen::BaseScreen *target, Graphics::Vec2D &size);
    void drawStringAt(Hardware::Screen::BaseScreen *target, const std::string &str, Graphics::Vec2D &position,
                      int16_t &maxCursorX, const Graphics::Vec2D &basePosition);

    std::string m_text;
    const FONT_INFO *m_fontInfo;
    HorizontalAlignment m_horizontalAlignment;

    WrapMode m_wrapMode;
    Graphics::Vec2D m_sizeLimit;

    Graphics::Vec2D m_size;

    size_t m_startChar;
    uint16_t m_startHeight;

    Graphics::Vec2D m_lastDrawPosition;
    Graphics::Vec2D m_lastSize;

    Graphics::Color m_textColor;
    Graphics::Color m_backgroundColor;

    uint16_t m_width;

}; // class Text

} // namespace Widget

#endif //PYRUS_TEXT_H
