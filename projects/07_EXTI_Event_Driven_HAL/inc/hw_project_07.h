/**************************************************************************************
* PROYECTO: 07_EXTI_Event_Driven_HAL
* ARCHIVO: hw_project_07.h
* DESCRIPCIÓN: Mapeo de hardware para el sistema orientado a eventos.
* Centraliza la conexión del bus LCD y los periféricos de control por interrupción.
**************************************************************************************/

#ifndef HW_PROJECT_07_H_
#define HW_PROJECT_07_H_

#include "gpio.h"

// --- ASIGNACIÓN DE BUS LCD (4-bits) ---

#define LCD_CTRL_PORT    GPIO_B
#define LCD_RS_PIN       0   // PB0 -> RS
#define LCD_EN_PIN       1   // PB1 -> Enable

#define LCD_DATA_PORT    GPIO_D
#define LCD_D4_PIN       4   // PD4
#define LCD_D5_PIN       5   // PD5
#define LCD_D6_PIN       6   // PD6
#define LCD_D7_PIN       7   // PD7


// --- ASIGNACIÓN DE PERIFÉRICOS ---

// LEDs de señalización
#define HEARTBEAT_LED    GPIO_B, 5  // Indicador de actividad (PB5)
#define RESPONSE_LED     GPIO_B, 3  // Indicador de evento EXTI (PB3)

// Entrada de disparo (Evento Externo)
/** @note Este pin (PD2) está vinculado físicamente a la línea INT0. */
#define BUTTON_INPUT     GPIO_D, 2  

#endif /* HW_PROJECT_07_H_ */