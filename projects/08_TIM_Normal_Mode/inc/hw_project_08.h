/**************************************************************************************
* PROYECTO: 08_TIM_Normal_Mode
* ARCHIVO: hw_project_08.h
* DESCRIPCIÓN: Mapeo integral de hardware para el control de motor PAP y HMI.
* Centraliza la orquesta de Timers (T0, T1, T2) y el bus de datos del LCD.
**************************************************************************************/

#ifndef HW_PROJECT_08_H_
#define HW_PROJECT_08_H_

/* --- 1. Dependencias y Drivers --- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

#include "gpio.h"             // Capa 1: GPIO
#include "systick.h"          // Capa 1: Timer 0 (1ms)
#include "timer1_normal.h"    // Capa 1: Timer 1 (16-bit Motor)
#include "timer2_normal.h"    // Capa 1: Timer 2 (8-bit LED)
#include "exti.h"             // Capa 1: Eventos Externos

#include "step_motor_28BYJ48.h" // Capa 2: Actuador
#include "lcd_driver.h"         // Capa 2: Display


/* --- 2. Mapeo del Motor Paso a Paso (28BYJ-48) --- */

#define M1_IN1_PORT    &PORTB
#define M1_IN1_PIN     PB0
#define M1_IN2_PORT    &PORTB
#define M1_IN2_PIN     PB1
#define M1_IN3_PORT    &PORTB
#define M1_IN3_PIN     PB2
#define M1_IN4_PORT    &PORTB
#define M1_IN4_PIN     PB4


/* --- 3. Mapeo del Bus LCD (Modo 4-bits) --- */

#define LCD_PORT_DIR   GPIO_C
#define LCD_RS_PIN     0   // PC0
#define LCD_EN_PIN     1   // PC1
#define LCD_D4_PIN     2   // PC2
#define LCD_D5_PIN     3   // PC3
#define LCD_D6_PIN     4   // PC4
#define LCD_D7_PIN     5   // PC5


/* --- 4. Interfaz de Usuario y Señalización --- */

// Entradas con funciones de Interrupción
#define BTN_START_STOP PD2   // INT0: Play/Stop
#define BTN_DIR        PD3   // INT1: Cambio de sentido

// LEDs de Estado
#define LED_BREATH     PB3   // LED controlado por Timer 2
#define LED_SYS_PORT   GPIO_D
#define LED_SYS_PIN    6     // LED de Systick


/* --- 5. Parámetros de Temporización Crítica --- */

/** * @brief Cálculo de velocidad del motor.
 * F_CPU = 16MHz, Prescaler = 64 -> 1 tick = 4us.
 * 625 ticks * 4us = 2.5ms por paso.
 */
#define MOTOR_SPEED_DEFAULT   625  

/** @brief Filtro de debounce para muestreo manual (20ms). */
#define BUTTON_DEBOUNCE_TICKS 5000 

#endif /* HW_PROJECT_08_H_ */