/**
 * @file main_project_09.h
 * @brief Prototipos de tareas y lógica de aplicación para el Proyecto 09.
 */

#ifndef MAIN_PROJECT_09_H_
#define MAIN_PROJECT_09_H_


/* --- 1. Definiciones de Preprocesador --- */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* --- 2. Inclusión de Hardware Específico --- */
#include "hw_project_09.h"


/**
 * @brief Tarea no bloqueante para el efecto breathing.
 */
void task_breathing(void);

/**
 * @brief Tarea no bloqueante para el incremento de brillo por pulsador.
 */
void task_button_led(void);

#endif /* MAIN_PROJECT_09_H_ */