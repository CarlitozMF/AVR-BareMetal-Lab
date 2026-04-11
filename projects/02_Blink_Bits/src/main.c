/**************************************************************************************
* PROYECTO: 02_Blink_bits
* AUTOR: Carlos Mamani Flores (UTN-FRT)
* DESCRIPCIÓN: Implementación de parpadeo de LEDs utilizando la Capa 0 (bits.h).
* Se abstrae la manipulación de registros mediante macros para mejorar la legibilidad.
* TOOLCHAIN: GCC (avr-gcc) + VS Code + Makefile
**************************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "bits.h"   /* Capa 0: Macros para manipulación atómica de bits */

int main(void) {

    /* --- Configuración de Hardware (Capa 0) --- */
    // Definimos PB0 y PB3 como salidas utilizando macros de escritura
    SET_BIT(DDRB, PB0);
    SET_BIT(DDRB, PB3);

    /* --- Bucle de Eventos (Super Loop) --- */
    while (1) {

        // 1. Encendido de LEDs: Seteo de bits individuales
        SET_BIT(PORTB, PB0);
        SET_BIT(PORTB, PB3);
        _delay_ms(100);

        // 2. Apagado de LEDs: Limpieza de bits individuales
        CLR_BIT(PORTB, PB0);
        CLR_BIT(PORTB, PB3);
        _delay_ms(100);

    } /* Fin del Super Loop */

    return 0; 
}