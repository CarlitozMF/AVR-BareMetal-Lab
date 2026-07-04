#ifndef APP_PROJECT_11_H
#define APP_PROJECT_11_H

#include <stdint.h>

/* Inicialización de toda la lógica de la aplicación */
void App_Init(void);

/* Tarea de control de movimiento (se llama en el loop) */
void App_Task_ServoSweep(void);

#endif