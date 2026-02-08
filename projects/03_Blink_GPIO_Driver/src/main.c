#include "gpio.h" // Solo necesitamos incluir el driver de la Capa 1
#include <util/delay.h>

int main(void) {
    // Inicialización elegante
    GPIO_InitPin(GPIO_B, 0, GPIO_OUTPUT);
    GPIO_InitPin(GPIO_B, 3, GPIO_OUTPUT);

    while (1) {
      /*
        GPIO_WritePin(GPIO_B, 0, GPIO_HIGH);
        GPIO_WritePin(GPIO_B, 3, GPIO_LOW);
        _delay_ms(150);*/

        GPIO_TogglePin(GPIO_B, 0);
        GPIO_TogglePin(GPIO_B, 3);
        _delay_ms(1000);
    }
    return 0;
}