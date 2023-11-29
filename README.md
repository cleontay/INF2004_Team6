# INF2004_Team6

## Note

The driver folder is for us to test our individual components with our Raspberry Pi Pico.
Drivers include:

1. **Wheel Encoder**
2. **IR Line sensor for Barcode and reading line**
3. **Magnometer**
4. **Motor**
5. **Ultrasonic**
6. **WiFi**
The codes in the driver folder are written in driver format to facilitate better integration.

The **integrated_car** folder is the integration of all our components using FreeRTOS.

## Robot Car Setup and Execution Guide

This guide provides step-by-step instructions for setting up and running the robot car code on the Raspberry Pi Pico using the Pico C SDK.

## Prerequisites

Before you begin, ensure you have the following:

- A Raspberry Pi Pico with assembled robot car (See below for component wiring)
- 3 IR Sensors, 2 Motors, 2 Wheel encoders, 1 L298N board, 1 Ultrasonic scanner, and a battery holder (With four batteries)
- [Pico C SDK](https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf) installed on your development machine.
- [Visual Studio Code](https://code.visualstudio.com/) installed on your development machine.
- [CMake](https://cmake.org/download/) installed on your development machine.
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extension installed in VSCode.


## Setting up the Visual Studio Code Environment

1. **Download or clone the project on github**

2. **Open the Project in VSCode:**

    - Open Visual Studio Code and navigate to the project folder.

3. **Configure CMake:**

    - Press `Ctrl+Shift+P` to open the command palette.
    - Type "CMake: Configure" and select it.

4. **Build the Project:**

    - Go to the CMake tab and press build on the target folder's elf file.

## Connecting and running code in the Raspberry Pi Pico

1. **Connect the Raspberry Pi Pico:**

    - Connect the Pico to your computer using a micro-USB cable.

2. **Run the code:**

    - Navigate to the build folder of the pico sdk build code, and find the target folder
    - Drag and drop the uf2 file within the target folder into the Raspberry Pi Pico directory.

## Robot Car Component linking and configuration

![Car Configuration](https://github.com/cleontay/INF2004_Team6/blob/main/integrated_car/car_configuration/car_configuration.jpg)


