cmake_minimum_required(VERSION 3.12)

project(Pyrus-fw C CXX ASM)

# define some variables just for this example to determine file locations
set(NRF_PROJECT_NAME Pyrus-fw)
set(NRF_BOARD pca10040)
set(NRF_SOFTDEVICE s132)

nRF5x_toolchainSetup()
nRF5x_setup()

#nRF5x_addAppScheduler()
#nRF5x_addAppFIFO()
nRF5x_addAppTimer()
#nRF5x_addAppUART()
nRF5x_addAppButton()
#nRF5x_addBSP(TRUE FALSE FALSE)
#
#nRF5x_addBLEService(ble_lbs)
nRF5x_addBLEAdvertising()
nRF5x_addBLEPeerManager()
nRF5x_addBLEGATT()
nRF5x_addBLEService(ble_dis)
nRF5x_addBLEService(ble_cts_c)

add_definitions(-DCONFIG_GPIO_AS_PINRESET)

include_directories(.)
list(APPEND SOURCE_FILES
        Applets/AppletManager.cpp
        Applets/BaseApplet.cpp
        Applets/DebugApt.cpp
        Applets/Tests/TestAMS.cpp
        Applets/Tests/TestApt.cpp
        Applets/Tests/TestClipping.cpp
        Applets/Tests/TestGfx.cpp
        Applets/Tests/TestWrap.cpp
        Events/BaseEvent.h
        Events/BackEvent.cpp
        Events/ButtonEvent.cpp
        Events/EventIdentifiers.h
        Events/EventManager.cpp
        Fonts/Ubuntu24Font.c
        Graphics/Bresenham.cpp
        Graphics/Color.h
        Graphics/GfxUtils.cpp
        Graphics/GfxUtils2.cpp
        Graphics/Vec2D.h
        Hardware/BLE/Clients/AppleMedia/AppleMediaNrf5.cpp
        Hardware/BLE/Clients/AppleMedia/BaseAppleMedia.cpp
        Hardware/BLE/Clients/CurrentTime/BaseCurrentTime.cpp
        Hardware/BLE/Clients/CurrentTime/CurrentTimeNrf5.cpp
        Hardware/BLE/BaseBle.cpp
        Hardware/BLE/BleNrf5.cpp
        Hardware/Button/BaseButton.cpp
        Hardware/Button/ButtonNrf52Dk.cpp
        Hardware/Button/ButtonPineTime.cpp
        Hardware/Clock/BaseClock.h
        Hardware/Clock/ClockNrf52.cpp
        Hardware/Screen/BaseScreen.h
        Hardware/Screen/ST7789.cpp
        Platform/BasePlatform.h
        Platform/Nrf52Dk.cpp
        Platform/PineTime.cpp
        Widgets/BaseContainer.cpp
        Widgets/BaseWidget.cpp
        Widgets/Container.cpp
        Widgets/StatusBar.cpp
        Widgets/Text.cpp
        Widgets/VerticalScrollContainer.cpp
        Application.cpp
        main.cpp)

nRF5x_addExecutable(Pyrus-fw "${SOURCE_FILES}")
target_link_libraries(Pyrus-fw m)