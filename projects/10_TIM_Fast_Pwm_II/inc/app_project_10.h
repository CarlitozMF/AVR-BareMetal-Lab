/**
 * @file app_project_10.h
 * @brief Definiciones de la lógica de aplicación y tareas del Proyecto 10.
 * @details Provee los prototipos de las tareas cooperativas y la función de 
 * inicialización del sistema (System Orchestrator).
 * @author Mamani Flores Carlos - CarlitozMF
 */

#ifndef APP_PROJECT_10_H_
#define APP_PROJECT_10_H_

#include <stdint.h>

/* --- 1. Inicialización del Sistema --- */

/**
 * @brief Configura y enlaza todos los servicios de la aplicación.
 * @details Realiza la inicialización del hardware (PWM, EXTI, GPIO) y 
 * la instancia del driver RGB con inyección de dependencias.
 */
void Sys_Init(void);

/* --- 2. Tareas Cooperativas (Super Loop) --- */

/**
 * @brief Tarea de efecto visual Rainbow (Arcoíris).
 * @details Máquina de estados no bloqueante que recorre el círculo cromático.
 * @param interval Tiempo de espera entre transiciones en milisegundos.
 * @param step Cantidad de unidades (0-255) a incrementar/decrementar por paso.
 */
void Task_Rainbow(uint32_t interval, uint8_t step);

/**
 * @brief Tarea de latido de corazón (Heartbeat).
 * @details Realiza un toggle en el LED de sistema para indicar que el 
 * programa no se ha bloqueado.
 * @param interval Tiempo en milisegundos entre cambios de estado.
 */
void Task_Toggle(uint32_t interval);

/**
 * @brief Tarea de control manual por pulsador.
 * @details Gestiona el brillo de un LED independiente (Dimmer) mediante 
 * una bandera levantada por la interrupción externa (EXTI).
 */
void task_button_led(void);

#endif /* APP_PROJECT_10_H_ */