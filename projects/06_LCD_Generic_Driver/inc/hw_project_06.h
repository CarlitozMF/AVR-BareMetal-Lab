/**************************************************************************************
* PROYECTO: 06_LCD_Generic_Driver
* ARCHIVO: hw_project_06.h
* DESCRIPCIÓN: Mapeo físico de pines y puertos del ATmega328P para este proyecto.
* Se centralizan las conexiones del LCD, LEDs de estado y pulsadores.
**************************************************************************************/

#ifndef HW_PROJECT_06_H_
#define HW_PROJECT_06_H_

#include "gpio.h"

// --- CONFIGURACIÓN DEL BUS LCD (Hitachi HD44780) ---

// Puerto de Control (RS y EN)
#define LCD_CTRL_PORT    GPIO_B
#define LCD_RS_PIN       0   // PB0 -> RS (Register Select)
#define LCD_EN_PIN       1   // PB1 -> EN (Enable)

// Puerto de Datos (4-bits: D4 a D7)
#define LCD_DATA_PORT    GPIO_D
#define LCD_D4_PIN       4   // PD4
#define LCD_D5_PIN       5   // PD5
#define LCD_D6_PIN       6   // PD6
#define LCD_D7_PIN       7   // PD7


// --- ASIGNACIÓN DE PERIFÉRICOS ---

// Indicadores visuales
#define HEARTBEAT_LED    GPIO_B, 5  // LED de estado del CPU (PB5)
#define RESPONSE_LED     GPIO_B, 3  // LED de respuesta al usuario (PB3)

// Entradas de usuario
#define BUTTON_INPUT     GPIO_D, 2  // Pulsador principal (PD2 - Soporta INT0)

#endif /* HW_PROJECT_06_H_ */