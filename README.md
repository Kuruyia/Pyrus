# Pyrus
Pyrus is yet another PineTime firmware whose goal is to run bare metal, without any kind of RTOS.  
This project is based on JF002's [nrf52-baseproject](https://github.com/JF002/nrf52-baseproject), which is a base to get CMake to work with the nRF52 MCU and SDK.

## What's working
- ST7789 LCD Controller, with support for hardware-assisted vertical scrolling
- nRF52's Real Time Counter, to keep track of the time
- nRF52's BLE, advertising and bonding works:
    - **AMS Client** to control the current media on iOS devices
    - **CTS Client** to synchronize the watch clock with the smartphone

## Building
You should follow the [instructions of the nrf52-baseproject](https://github.com/JF002/nrf52-baseproject/blob/master/README.md#nrf52-baseproject) to get a build environment ready to compile Pyrus.  
This project is currently being compiled using [gcc-arm-none-eabi-8-2019-q3-update](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) (GCC 9 is known **not** to work), and the [nRF5 SDK 16.0.0](https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download#infotabs).

Once your environment is ready, you just have to run the following commands to compile this firmware:
```sh
git clone https://github.com/Arc13/Pyrus.git
cd Pyrus
mkdir build && cd build
cmake -DARM_NONE_EABI_TOOLCHAIN_PATH=[...] -DNRF5_SDK_PATH=[...] -DNRFJPROG=[...] ..
```

If you haven't already, you need to flash the SoftDevice **only once**:
```sh
cmake --build . --target FLASH_SOFTDEVICE
```

To compile the firmware, you can run this command:
```sh
cmake --build . --target Pyrus-fw
```

You will then have the generated firmware image available under the ``src/`` directory. 

## Flashing
If your PineTime is directly connected to your machine (via, for instance, the nRF52-DK integrated SEGGER JLink debugger/programmer),
you can flash this image to the smartwatch with the following command:
```sh
nrfjprog --program Pyrus-fw.hex --sectorerase
```

## Acknowledgments
- JF002 for creating the [nrf52-baseproject](https://github.com/JF002/nrf52-baseproject) to easily get started on the nRF52 with CMake and CLion
- Pine64 and The PineTime community for all the precious information written in [the wiki](https://wiki.pine64.org/index.php/PineTime)
- Canonical for the [Ubuntu font](https://design.ubuntu.com/font/) (License for this font is provided [here](https://github.com/Arc13/Pyrus/tree/master/src/Fonts/ubuntu-font-licence-1.0.txt))
- The [How to Code Your First Algorithm — Draw A Line](https://www.freecodecamp.org/news/how-to-code-your-first-algorithm-draw-a-line-ca121f9a1395/) article and the Adafruit GFX library for,
respectively, the function to draw a line and a triangle