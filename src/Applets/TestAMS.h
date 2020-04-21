#ifndef PYRUS_TESTAMS_H
#define PYRUS_TESTAMS_H

#include "../Widgets/Text.h"
#include "BaseApplet.h"

namespace Applet {

class TestAMS: public BaseApplet {
public:
    explicit TestAMS(Hardware::BLE::Clients::AppleMediaNrf5 &appleMedia);

    void processEvent() override;
    void update(Platform::BasePlatform &platform) override;
    void draw(Hardware::Screen::BaseScreen &target) override;

    bool allowsStatusBar() const override;

private:
    void updateInfoTexts();
    void updateMetaText();

    Hardware::BLE::Clients::AppleMediaNrf5 &m_appleMedia;

    std::string m_songTitle;
    std::string m_songArtist;
    std::string m_songAlbum;

    std::string m_songDuration;
    float m_deviceVolume;

    Widget::Text m_amsText;
    Widget::Text m_titleText;
    Widget::Text m_subtitleText;
    Widget::Text m_metaText;
};

}

#endif //PYRUS_TESTAMS_H
