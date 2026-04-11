/**************************************************************************************
* PROYECTO: 06_LCD_Generic_Driver
* ARCHIVO: main_project_06.h
* DESCRIPCIÓN: Definiciones de constantes de sistema, prototipos de tareas y 
* dependencias de drivers para la lógica de aplicación.
**************************************************************************************/

#ifndef MAIN_PROJECT_06_H_
#define MAIN_PROJECT_06_H_

/* --- Configuración de Reloj --- */
#ifndef F_CPU
#define F_CPU 16000000UL  // Frecuencia de oscilador externo (16MHz)
#endif

/* --- Dependencias de Capas (HAL y Drivers) --- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "hw_project_06.h" // Capa 1: Mapeo de Hardware
#include "systick.h"       // Capa 1: Base de tiempo de 1ms
#include "lcd_driver.h"    // Capa 2: Driver de pantalla

/* --- Recursos Compartidos --- */

/** * @brief Patrón de bits para el carácter personalizado "Rayo".
 * @note Definido en main.c y exportado para su uso en la inicialización.
 */
extern uint8_t char_rayo[8];


/* --- Prototipos de Funciones de Aplicación (Capa 3) --- */

/** @brief Inicialización integral de Hardware, HAL y Dispositivos. */
void System_Init(void);

/** @brief Tarea de latido (Heartbeat): Toggle de LED cada 200ms. */
void Task_Heartbeat(void);

/** @brief Tarea de cronómetro: Actualiza el Uptime en el LCD cada 1s. */
void Task_Contador(void);

/** @brief Tarea de interfaz: Escaneo de pulsador y control de estado/HMI. */
void Task_Boton(void);

#endif /* MAIN_PROJECT_06_H_ */