/**************************************************************************************
* PROYECTO: 08_TIM_Normal_Mode
* ARCHIVO: main_project_08.h
* DESCRIPCIÓN: Definiciones de constantes de tiempo para tareas de interfaz, 
* prototipos de funciones de sistema y exportación de recursos globales.
**************************************************************************************/

#ifndef MAIN_PROJECT_08_H_
#define MAIN_PROJECT_08_H_

/* --- 1. Definiciones de Preprocesador --- */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* --- 2. Inclusión de Hardware Específico --- */
#include "hw_project_08.h"

/* --- 3. Constantes de Tiempo para el Scheduler (Capa 3) --- */

// Intervalo de actualización del LCD (200ms para evitar flickering)
#define T_REFRESH_LCD_MS   200  

// Intervalo de parpadeo del LED de Sistema (250ms -> 2Hz)
#define T_BLINK_SYS_MS     250  

// Frecuencia del Heartbeat (Timer 2): 61 overflows aprox. 1 segundo
#define T2_OVF_COUNT_1S    61   


/* --- 4. Prototipos de Funciones de Aplicación --- */

/**
 * @brief Inicialización integral de todos los periféricos.
 * @details Configura GPIOs, Motores, LCD, EXTI y la orquesta de Timers (0, 1 y 2).
 */
void System_Init(void);

/**
 * @brief Actualización de la interfaz visual en el LCD.
 * @param running Estado actual de marcha del motor.
 * @param dir Sentido de giro actual.
 */
void Task_Update_HMI(bool running, Step_Dir_t dir);


/* --- 5. Declaraciones Externas (Opcional si se modulariza) --- */

// Si movieras las ISR a otro archivo, estas variables deberían ser extern:
// extern volatile bool motor_running;
// extern volatile Step_Dir_t motor_dir;

#endif /* MAIN_PROJECT_08_H_ */