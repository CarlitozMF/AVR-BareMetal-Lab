/**************************************************************************************
* PROYECTO: 04_GPIO_Polling_&_Debouncing
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Control de un LED mediante un pulsador externo con lógica negativa. 
* Implementa un sistema de anti-rebote (Debounce) por software y una bandera de 
* estado para evitar múltiples disparos en una sola pulsación.
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/

#include "gpio.h"        /* Capa 1: Driver de GPIO */
#include <util/delay.h>   /* Funciones de tiempo */

int main(void) {

    /* --- Configuración de Hardware --- */
    GPIO_InitPin(GPIO_B, 0, GPIO_OUTPUT); // LED de estado
    GPIO_InitPin(GPIO_D, 2, GPIO_INPUT);  // Pulsador en PD2
    
    /* Nota: En AVR, escribir HIGH en un pin de entrada activa la 
       resistencia de Pull-up interna (evita estados flotantes). */
    GPIO_WritePin(GPIO_D, 2, GPIO_HIGH);  

    // Bandera (Flag) para detectar el flanco de presión y evitar repeticiones
    uint8_t boton_presionado = 0; 

    /* --- Bucle de Eventos (Super Loop) --- */
    while (1) {
        
        // 1. Detección de pulsación (Lógica negativa: Presionado = LOW)
        if (GPIO_ReadPin(GPIO_D, 2) == GPIO_LOW) {
            
            // 2. Control de flanco: Solo actuamos si el botón no estaba presionado antes
            if (!boton_presionado) {
                GPIO_TogglePin(GPIO_B, 0); // Acción: Cambiar estado del LED
                
                boton_presionado = 1;      // Bloqueamos futuras entradas hasta que suelte
                
                /* Debounce: Retardo para ignorar los ruidos mecánicos 
                   del contacto metálico del pulsador. */
                _delay_ms(50);             
            }
        } 
        else {
            // 3. Liberación: Si el pin está HIGH, el usuario soltó el botón
            boton_presionado = 0;
        }

    } /* Fin del Super Loop */

    return 0; 
}