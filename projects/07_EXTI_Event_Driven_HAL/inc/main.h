#ifndef MAIN_H_
#define MAIN_H_

/* 1. Definiciones de Preprocesador (La base de todo) */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* 2. Librerías del Sistema (Standard C y AVR-LibC) */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

/* 3. Capa 0 y Configuración de Hardware (Necesario para los drivers) */
#include "bits.h"
#include "hardware.h"  // <--- Subilo aquí. hardware.h ya incluye a gpio.h

/* 4. Capa 1 (HAL) y Capa 2 (Devices) */
#include "exti.h"
#include "timer.h"
#include "lcd_driver.h"

/* Carácter personalizado: Rayo ⚡ */
extern uint8_t char_rayo[8];

void System_Init(void);
void Task_Heartbeat(void);
void Task_Contador(void);

#endif