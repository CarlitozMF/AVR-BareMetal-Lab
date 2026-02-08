#include "gpio.h"
#include <util/delay.h>

int main(void) {
    // Configuración
    GPIO_InitPin(GPIO_B, 0, GPIO_OUTPUT); // LED
    GPIO_InitPin(GPIO_D, 2, GPIO_INPUT);  // Pulsador
    GPIO_WritePin(GPIO_D, 2, GPIO_HIGH);  // Pull-up interna activa

    uint8_t boton_presionado = 0; // Bandera de estado (Flag)

    while (1) {
        // 1. Detectamos si el botón está presionado (Lógica negativa)
        if (GPIO_ReadPin(GPIO_D, 2) == GPIO_LOW) {
            
            // 2. ¿Es la primera vez que detectamos esta pulsación?
            if (!boton_presionado) {
                GPIO_TogglePin(GPIO_B, 0); // Cambiamos el estado del LED
                boton_presionado = 1;      // Marcamos que ya procesamos este click
                _delay_ms(50);             // Debounce simple para evitar ruidos
            }
        } else {
            // 3. Cuando el usuario suelta el botón, reseteamos la bandera
            boton_presionado = 0;
        }
    }
    return 0;
}