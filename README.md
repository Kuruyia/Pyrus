# Pyrus
Pyrus is yet another PineTime firmware whose goal is to run bare metal, without any kind of RTOS.  
This project is based on JF002's [nrf52-baseproject](https://github.com/JF002/nrf52-baseproject), which is a base to get CMake to work with the nRF52 MCU and SDK.

## Building
You should follow the [instructions of the nrf52-baseproject](https://github.com/JF002/nrf52-baseproject/wiki/Build,-program-and-debug-NRF52-project-with-JLink,-CMake-and-CLion) to get a build environment ready to compile Pyrus.  
This project is currently being compiled using [gcc-arm-none-eabi-8-2019-q3-update](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) (GCC 9 is known **not** to work), and the [nRF5 SDK 16.0.0](https://www.nordicsemi.com/Software-and-tools/Software/nRF5-SDK/Download#infotabs).

Once your environment is ready, you just have to run the following commands to compile this firmware:
```sh
git clone https://github.com/Arc13/Pyrus.git
cd Pyrus
mkdir build && cd build
cmake -DARM_NONE_EABI_TOOLCHAIN_PATH=[...] -DNRF5_SDK_PATH=[...] -DNRFJPROG=[...] ..
cmake --build . --target Pyrus-fw
```

You will then have the generated firmware image available under the ``src/`` directory. 

## Flashing
If your PineTime is directly connected to your machine (via, for instance, the nRF52-DK integrated SEGGER JLink debugger/programmer),
you can flash this image to the smartwatch with the following command:
```sh
nrfjprog --program Pyrus-fw.hex --sectorerase
```