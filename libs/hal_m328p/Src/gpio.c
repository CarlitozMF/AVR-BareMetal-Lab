/**
 * @file gpio.c
 * @brief Implementación del Driver de GPIO para ATmega328P.
 * @author Mamani Flores Carlos
 */

#include "gpio.h"

/* Definición física de los puertos */
const gpio_port_t GPIO_B = { .DDR = &DDRB, .PORT = &PORTB, .PIN = &PINB };
const gpio_port_t GPIO_C = { .DDR = &DDRC, .PORT = &PORTC, .PIN = &PINC };
const gpio_port_t GPIO_D = { .DDR = &DDRD, .PORT = &PORTD, .PIN = &PIND };

/**
 * @brief Configura la dirección del pin (Entrada/Salida)
 */
void GPIO_InitPin(gpio_port_t port, uint8_t pin, gpio_mode_t mode) {
    if (mode == GPIO_OUTPUT) {
        SET_BIT(*(port.DDR), pin);
    } else {
        CLR_BIT(*(port.DDR), pin);
    }
}

/**
 * @brief Escribe un nivel lógico en el pin
 */
void GPIO_WritePin(gpio_port_t port, uint8_t pin, gpio_state_t state) {
    if (state == GPIO_HIGH) {
        SET_BIT(*(port.PORT), pin);
    } else {
        CLR_BIT(*(port.PORT), pin);
    }
}

/**
 * @brief Alterna el estado del pin (Toggle)
 */
void GPIO_TogglePin(gpio_port_t port, uint8_t pin) {
    TOGGLE_BIT(*(port.PORT), pin);
}

/**
 * @brief Lee y retorna el estado de un pin
 */
gpio_state_t GPIO_ReadPin(gpio_port_t port, uint8_t pin) {
    if (BIT_IS_SET(*(port.PIN), pin)) {
        return GPIO_HIGH;
    } else {
        return GPIO_LOW;
    }
}