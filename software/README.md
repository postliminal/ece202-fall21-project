# Software

## Project files and organization

- ble_app: localization application using ble
  - ble_beacon_app_dongle & ble_beacon_app_arduino: keystone for localization
  - ble_central_app_arduino: central device calculating location and listening for wake word
  - *dk: dev kit versions for development and debugging
- openthread_app: localization application using openthread
  - ot_beacon_app_dongle & ot_beacon_app_arduino: keystone for localization
  - ot_central_app_arduino: central device calculating location and listening for wake word
  - *dk: dev kit versions for development and debugging

## Instructions for development/building projects

As it is well known, there are several tools for development and building applications for boards
using the nRF52840 SoC, so you should use the tools you're comfortable with. 
Here we outline the specific setup that worked best for us.

Reference: [element14 blog post](https://community.element14.com/products/roadtest/b/blog/posts/tutorial-02-part-1-developing-nrf52840-application-arduino-nano-33-ble-sense-roadtest)

1. Download the following tools
  - Nordic SDKs: sample code+drivers+header files from hardware provider
    - [nRF5 SDK](https://www.nordicsemi.com/Products/Development-software/nRF5-SDK)
    - [nRF5 SDK for Thread and Zigbee](https://www.nordicsemi.com/Products/Development-software/nRF5-SDK-for-Thread-and-Zigbee)
  - Nordic nRF Connect: for uploading code to the board
    - [nRF Connect](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-desktop)
  - Nordic Softdevice s140
    - [s140](https://www.nordicsemi.com/Products/Development-software/S140)
  - Segger Embedded Studio: IDE for development - includes tools for debugging
    - [SES](https://www.segger.com/downloads/embedded-studio)
      - After installation go to _Tools>Package Manager_ and install the nRF CPU Support Package.
2. Modify SDK paths in *.emProject files
  - Using find+replace tools from a text editor, replace "/Users/royjara/Desktop/nRF5_SDK_17.1.0_ddde560"
with your own path to the SDK
