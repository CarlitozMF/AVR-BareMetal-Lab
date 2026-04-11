/**************************************************************************************
* PROYECTO: 07_EXTI_Event_Driven_HAL
* ARCHIVO: main_project_07.h
* DESCRIPCIÓN: Definiciones globales, prototipos de tareas y configuración de la 
* base de tiempo y eventos externos para el sistema reactivo.
**************************************************************************************/

#ifndef MAIN_PROJECT_07_H_
#define MAIN_PROJECT_07_H_

/* --- 1. Definiciones de Preprocesador --- */
#ifndef F_CPU
#define F_CPU 16000000UL // Frecuencia de trabajo para cálculos de tiempo
#endif

/* --- 2. Librerías del Sistema --- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

/* --- 3. Capa 0 y Configuración de Hardware --- */
#include "bits.h"            // Macros atómicas
#include "hw_project_07.h"    // Mapeo físico de pines

/* --- 4. Capa 1 (HAL) y Capa 2 (Drivers de Dispositivos) --- */
#include "exti.h"            // Interrupciones externas
#include "systick.h"         // Base de tiempo de 1ms
#include "lcd_driver.h"      // Control de HMI

/* --- 5. Recursos Compartidos y Datos Externos --- */

/** @brief Patrón de bits para el carácter personalizado 0 (Rayo). */
extern uint8_t char_rayo[8];


/* --- 6. Prototipos de Funciones (Lógica de Aplicación) --- */

/** @brief Inicialización de periféricos, configuración de EXTI y bienvenida HMI. */
void System_Init(void);

/** @brief Tarea Heartbeat: Parpadeo de LED cada 200ms para control de CPU. */
void Task_Heartbeat(void);

/** @brief Tarea de cronómetro: Actualización de Uptime en pantalla cada 1s. */
void Task_Contador(void);

#endif /* MAIN_PROJECT_07_H_ */