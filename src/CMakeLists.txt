cmake_minimum_required(VERSION 3.12)

project(Pyrus-fw C CXX ASM)

# define some variables just for this example to determine file locations
set(NRF_PROJECT_NAME Pyrus-fw)
set(NRF_BOARD pca10040)
#set(NRF_SOFTDEVICE s132)

nRF5x_toolchainSetup()
nRF5x_setup()

#nRF5x_addAppScheduler()
#nRF5x_addAppFIFO()
nRF5x_addAppTimer()
#nRF5x_addAppUART()
nRF5x_addAppButton()
#nRF5x_addBSP(TRUE FALSE FALSE)
#nRF5x_addBLEGATT
#
#nRF5x_addBLEService(ble_lbs)

add_definitions(-DCONFIG_GPIO_AS_PINRESET)

include_directories(.)
list(APPEND SOURCE_FILES
        Fonts/Ubuntu24Font.c
        Graphics/Color.h
        Graphics/Vec2D.h
        Hardware/Screen/BaseScreen.h
        Hardware/Screen/ST7789.cpp
        Widgets/BaseContainer.cpp
        Widgets/BaseWidget.cpp
        Widgets/Container.cpp
        Widgets/Text.cpp
        Widgets/VerticalScrollContainer.cpp
        main.cpp)

nRF5x_addExecutable(Pyrus-fw "${SOURCE_FILES}")