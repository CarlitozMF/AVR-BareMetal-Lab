/**************************************************************************************
* PROYECTO: 11_SERVO_SG90_ARCHITECTURE
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Control de múltiples servos usando arquitectura de 4 capas.
* Capa 1: Timer 1 Fast PWM (16-bits)
* Capa 2: Driver Genérico SG90 (Inyección de dependencias)
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/
#include <avr/interrupt.h>
#include "app_project_11.h"

int main(void) {
    /* Inicialización de la aplicación */
    App_Init();

    /* Habilitar interrupciones globales */
    sei();

    while (1) {
        /* Despacho de tareas cooperativas */
        App_Task_ServoSweep();
    }

    return 0;
}