/**
 * 
 * REFERENCE: https://github.com/drewvigne/arduino_nano_33_ant/blob/master/boards/arduino_nano_33_ant.h
 * 
 * Copyright (c) 2016 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef PCA10056_ARDUINO_H
#define PCA10056_ARDUINO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// LEDs definitions for PCA10056_arduino
#define LEDS_NUMBER                 5

#define LED_1                       NRF_GPIO_PIN_MAP(1,9)  // PWR
#define LED_2                       NRF_GPIO_PIN_MAP(0,24) // RED
#define LED_3                       NRF_GPIO_PIN_MAP(0,16) // GREEN
#define LED_4                       NRF_GPIO_PIN_MAP(0,6)  // BLUE
#define LED_5                       NRF_GPIO_PIN_MAP(0,13) // BUILTIN
#define LED_START                   LED_1
#define LED_STOP                    LED_5

#define LEDS_ACTIVE_STATE 0

#define LEDS_LIST { LED_1, LED_2, LED_3, LED_4, LED_5 }

#define LEDS_INV_MASK  LEDS_MASK

#define BSP_LED_0                   LED_1
#define BSP_LED_1                   LED_2
#define BSP_LED_2                   LED_3
#define BSP_LED_3                   LED_4
#define BSP_LED_4                   LED_5

// Button Definitions
#define BUTTONS_NUMBER              4

// #define BUTTON_START                NRF_GPIO_PIN_MAP(0,4) // Nano 33 has no buttons so use analog pins to ground
#define BUTTON_1                    NRF_GPIO_PIN_MAP(0,4)
#define BUTTON_2                    NRF_GPIO_PIN_MAP(0,5)
#define BUTTON_3                    NRF_GPIO_PIN_MAP(0,30)
#define BUTTON_4                    NRF_GPIO_PIN_MAP(0,29)
// #define BUTTON_STOP                 NRF_GPIO_PIN_MAP(0,29)
#define BUTTON_PULL                 NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4  }

#define BSP_BUTTON_0   BUTTON_1
#define BSP_BUTTON_1   BUTTON_2
#define BSP_BUTTON_2   BUTTON_3
#define BSP_BUTTON_3   BUTTON_4

// IO Definitions
#define RX_PIN_NUMBER               NRF_GPIO_PIN_MAP(1,10)
#define TX_PIN_NUMBER               NRF_GPIO_PIN_MAP(1,3)
#define CTS_PIN_NUMBER              19
#define RTS_PIN_NUMBER              20
#define HWFC                        false // APP_UART_FLOW_CONTROL_DISABLED

#define BSP_QSPI_SCK_PIN            NRF_GPIO_PIN_MAP(0,19)
#define BSP_QSPI_CSN_PIN            NRF_GPIO_PIN_MAP(0,17)
#define BSP_QSPI_IO0_PIN            NRF_GPIO_PIN_MAP(0,20)
#define BSP_QSPI_IO1_PIN            NRF_GPIO_PIN_MAP(0,22) // VDD_ENV
#define BSP_QSPI_IO2_PIN            NRF_GPIO_PIN_MAP(0,21)
#define BSP_QSPI_IO3_PIN            NRF_GPIO_PIN_MAP(0,23)

#define SPIS_MISO_PIN               NRF_GPIO_PIN_MAP(1,8)  // SPI MISO signal
#define SPIS_CSN_PIN                NRF_GPIO_PIN_MAP(1,2)  // SPI CSN signal
#define SPIS_MOSI_PIN               NRF_GPIO_PIN_MAP(1,1)  // SPI MOSI signal
#define SPIS_SCK_PIN                NRF_GPIO_PIN_MAP(0,13) // SPI SCK signal

// I2C Definitions
#define VDD_ENV_PIN                 NRF_GPIO_PIN_MAP(0,22)
#define R_PULLUP_PIN                NRF_GPIO_PIN_MAP(1,0)

// Arduino Pin defs
#define ARDUINO_SCL_PIN             NRF_GPIO_PIN_MAP(0,2)  // SCL signal pin
#define ARDUINO_SDA_PIN             NRF_GPIO_PIN_MAP(0,31) // SDA signal pin
#define ARDUINO_SCL1_PIN            NRF_GPIO_PIN_MAP(0,15) // SCL1 signal pin
#define ARDUINO_SDA1_PIN            NRF_GPIO_PIN_MAP(0,14) // SDA1 signal pin
#define ARDUINO_AREF_PIN            21    		   // Aref pin

#define ARDUINO_13_PIN              NRF_GPIO_PIN_MAP(0, 13)  // Digital pin 13
#define ARDUINO_12_PIN              NRF_GPIO_PIN_MAP(1, 8)  // Digital pin 12
#define ARDUINO_11_PIN              NRF_GPIO_PIN_MAP(1, 1)  // Digital pin 11
#define ARDUINO_10_PIN              NRF_GPIO_PIN_MAP(1, 2)  // Digital pin 10
#define ARDUINO_9_PIN               NRF_GPIO_PIN_MAP(0, 27)  // Digital pin 9
#define ARDUINO_8_PIN               NRF_GPIO_PIN_MAP(0, 21)  // Digital pin 8

#define ARDUINO_7_PIN               NRF_GPIO_PIN_MAP(0, 23) // Digital pin 7
#define ARDUINO_6_PIN               NRF_GPIO_PIN_MAP(1, 14) // Digital pin 6
#define ARDUINO_5_PIN               NRF_GPIO_PIN_MAP(1, 13) // Digital pin 5
#define ARDUINO_4_PIN               NRF_GPIO_PIN_MAP(1, 15) // Digital pin 4
#define ARDUINO_3_PIN               NRF_GPIO_PIN_MAP(1, 12) // Digital pin 3
#define ARDUINO_2_PIN               NRF_GPIO_PIN_MAP(1, 11) // Digital pin 2
#define ARDUINO_1_PIN               NRF_GPIO_PIN_MAP(1, 10) // Digital pin 1
#define ARDUINO_0_PIN               NRF_GPIO_PIN_MAP(1, 3) // Digital pin 0

#define ARDUINO_A0_PIN              NRF_GPIO_PIN_MAP(0,4)  // Analog channel 0
#define ARDUINO_A1_PIN              NRF_GPIO_PIN_MAP(0,5)  // Analog channel 1
#define ARDUINO_A2_PIN              NRF_GPIO_PIN_MAP(0,30) // Analog channel 2
#define ARDUINO_A3_PIN              NRF_GPIO_PIN_MAP(0,29) // Analog channel 3
#define ARDUINO_A4_PIN              NRF_GPIO_PIN_MAP(0,31) // Analog channel 4
#define ARDUINO_A5_PIN              NRF_GPIO_PIN_MAP(0,2)  // Analog channel 5
#define ARDUINO_A6_PIN              NRF_GPIO_PIN_MAP(0,28) // Analog channel 6
#define ARDUINO_A7_PIN              NRF_GPIO_PIN_MAP(0,3)  // Analog channel 7


#ifdef __cplusplus
}
#endif

#endif // PCA10056_H
