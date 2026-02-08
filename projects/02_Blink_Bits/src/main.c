#include <avr/io.h>
#include <util/delay.h>
#include "bits.h"

int main(void) {
    /* Configuración: Salidas para PB0 y PB3 */
    SET_BIT(DDRB, PB0);
    SET_BIT(DDRB, PB3);

    while (1) {
        /* Encender LEDs */
        SET_BIT(PORTB, PB0);
        SET_BIT(PORTB, PB3);
        _delay_ms(100);

        /* Apagar LEDs */
        CLR_BIT(PORTB, PB0);
        CLR_BIT(PORTB, PB3);
        _delay_ms(100);
    }
    return 0;
}