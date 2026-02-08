#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "gpio.h"

/* --- LCD Pines (Originales) --- */
#define LCD_CTRL_PORT    GPIO_B
#define LCD_DATA_PORT    GPIO_D

#define LCD_RS_PIN       0   // PB0 (Reset/RS)
#define LCD_EN_PIN       1   // PB1 (Enable)

#define LCD_D4_PIN       4   // PD4
#define LCD_D5_PIN       5   // PD5
#define LCD_D6_PIN       6   // PD6
#define LCD_D7_PIN       7   // PD7

/* --- Periféricos --- */
#define HEARTBEAT_LED    GPIO_B, 5  // PB5
#define RESPONSE_LED     GPIO_B, 3  // PB3
#define BUTTON_INPUT     GPIO_D, 2  // PD2 (INT0)

#endif