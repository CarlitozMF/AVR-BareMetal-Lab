/*
**************************************************************************************
* PROYECTO: 01_Blink_BareMetal
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Configuración básica de GPIO y manejo de retardos bloqueantes para 
* hacer parpadear dos LEDs en los pines PB0 y PB3.
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************
*/

#define F_CPU 16000000UL 

/* --- Preámbulo --- */
#include <avr/io.h>         /* Definiciones de registros, pines y puertos */
#include <util/delay.h>      /* Funciones de tiempo basadas en ciclos de reloj */

int main(void) {

    /* --- Configuración de Hardware (Capa 0/1) --- */
    // Configuramos PB0 y PB3 como salidas (1) en el registro de dirección
    DDRB |= (1 << PB0) | (1 << PB3); 

    /* --- Bucle de Eventos (Super Loop) --- */
    while (1) {

        // 1. Encendido de LEDs: Seteamos los bits correspondientes en el puerto
        PORTB |= (1 << PB0) | (1 << PB3); 
        _delay_ms(500); 

        // 2. Apagado de LEDs: Limpiamos los bits mediante máscara AND y NOT
        PORTB &= ~((1 << PB0) | (1 << PB3)); 
        _delay_ms(500); 

    } /* Fin del Super Loop */

    return 0; // El flujo nunca llegará aquí
}