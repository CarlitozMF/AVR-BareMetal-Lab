/**************************************************************************************
* PROYECTO: 03_Blink_GPIO_Driver
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Uso avanzado del Driver de GPIO (Capa 1). Implementación de parpadeo
* mediante la función Toggle, optimizando la lógica del Super Loop.
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/

#include "gpio.h"        /* Capa 1: Driver de Abstracción de GPIO */
#include <util/delay.h>   /* Funciones de tiempo */

int main(void) {

    /* --- Inicialización Elegante (Capa 1) --- */
    // Configuramos los pines PB0 y PB3 como salidas digitales
    GPIO_InitPin(GPIO_B, 0, GPIO_OUTPUT);
    GPIO_InitPin(GPIO_B, 3, GPIO_OUTPUT);

    /* --- Bucle de Eventos (Super Loop) --- */
    while (1) {

        /* * Nota: Se opta por GPIO_TogglePin en lugar de WritePin para 
         * simplificar la lógica de control de estado.
         */
        
        // Invierte el estado lógico actual de los pines PB0 y PB3
        GPIO_TogglePin(GPIO_B, 0);
        GPIO_TogglePin(GPIO_B, 3);
        
        // Delay de 1 segundo (1000ms)
        _delay_ms(1000);

    } /* Fin del Super Loop */

    return 0; 
}